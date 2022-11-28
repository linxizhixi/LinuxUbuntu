#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

#define DATA_MASK  ((1 << 18) - 1)
#define DATA_SHIFT 8

#define DATA_BUF_SIZE 400

char device_name[128];
unsigned int aun_red_buf[DATA_BUF_SIZE];
unsigned int aun_ir_buf[DATA_BUF_SIZE];

extern void maxim_heart_rate_and_oxygen_saturation(unsigned int *pun_ir_buffer ,  int n_ir_buffer_length, unsigned int *pun_red_buffer ,   int *pn_spo2, int *pch_spo2_valid ,  int *pn_heart_rate , int  *pch_hr_valid);

int write_sys_int(char *filename, int data)
{
	int ret = 0;
	FILE *filp;

	//printf("filename = %s\n", filename);

	filp = fopen(filename, "w");
	if (!filp) {
		ret = -errno;
		fprintf(stderr, "failed to open %s\n", filename);
		goto error;
	}

	ret = fprintf(filp, "%d", data);
	if (ret < 0) {
		if (fclose(filp))
			perror("_write_sysfs_int(): Failed to close dir");

		goto error;
	}

	if (fclose(filp)) {
		ret = -errno;
		goto error;
	}

	return 0;

error:
	return ret;
}

int enable_disable_all_channels(int enable)
{
	int ret = 0;
	char red_enable[128] = {0};	
	char ir_enable[128] = {0};	

	sprintf(red_enable, "/sys/bus/iio/devices/%s/scan_elements/in_intensity_red_en", device_name);
	sprintf(ir_enable, "/sys/bus/iio/devices/%s/scan_elements/in_intensity_ir_en", device_name);

	ret = write_sys_int(red_enable, enable);
	if (ret < 0) {
		fprintf(stderr, "Failed to enable/disable %s", red_enable);
		return ret;
	}

	ret = write_sys_int(ir_enable, enable);
	if (ret < 0) {
		fprintf(stderr, "Failed to enable/disable %s", ir_enable);
		return ret;
	}

	return 0;
}

int enable_disable_buffer(int enable)
{
	int ret = 0;
	char buffer_enable[128] = {0};

	sprintf(buffer_enable, "/sys/bus/iio/devices/%s/buffer/enable", device_name);

	ret = write_sys_int(buffer_enable, enable);
	if (ret < 0) {
		fprintf(stderr, "Failed to enable/disable %s", buffer_enable);
		return ret;
	}

	return 0;
}

int set_buffer_len(int len)
{
	int ret = 0;
	char buffer_len[128] = {0};

	sprintf(buffer_len, "/sys/bus/iio/devices/%s/buffer/length", device_name);

	ret = write_sys_int(buffer_len, len);
	if (ret < 0) {
		fprintf(stderr, "Failed to enable/disable %s", buffer_len);
		return ret;
	}

	return 0;
}

void cleanup()
{
	enable_disable_buffer(0);
	enable_disable_all_channels(0);
}

void sig_handler(int signum)
{
	fprintf(stderr, "Caught signal %d\n", signum);
	cleanup();
	exit(-signum);
}

void register_cleanup(void)
{
	struct sigaction sa = { .sa_handler = sig_handler };
	const int signums[] = { SIGINT, SIGTERM, SIGABRT };
	int ret, i;

	for (i = 0; i < 3; ++i) {
		ret = sigaction(signums[i], &sa, NULL);
		if (ret) {
			perror("Failed to register signal handler");
			exit(-1);
		}
	}
}



int main(int argc, const char *argv[])
{
	int ret = 0;
	int data[2];
	char device_path[128] = {0};
	int fd = 0;
	int i, j;
	int read_size;

	int SpO2;
	int heart_rate;
	int hr_valid;
	int spo2_valid;
	int tmp_val = 0;

	if (argc != 2) {
		printf("usage: %s <iio:device[n]>\n", argv[0]);
		return 1;
	}

	register_cleanup();

	sprintf(device_name, "%s", argv[1]);

	enable_disable_all_channels(1);
	set_buffer_len(4);
	enable_disable_buffer(1);
	
	sprintf(device_path, "/dev/%s", device_name);

	fd = open(device_path, O_RDONLY | O_NONBLOCK);
	if (fd < 0) { /* TODO: If it isn't there make the node */
		fprintf(stderr, "Failed open device: %s", strerror(errno));
		goto error;
	}


	for (j = 0; j < DATA_BUF_SIZE; j++) {
		struct pollfd pfd = {
			.fd = fd,
			.events = POLLIN,
		};

		ret = poll(&pfd, 1, -1);
		if (ret < 0) {
			ret = -errno;
			goto error;
		} else if (ret == 0) {
			printf("continue\n");
			continue;
		}

		read_size = read(fd, data, 8);
		if (read_size < 0) {
			if (errno == EAGAIN) {
				fprintf(stderr, "nothing available\n");
				continue;
			} else {
				break;
			}
		}


		tmp_val = be32toh(data[0]);
		aun_red_buf[j] = (tmp_val >> DATA_SHIFT) & DATA_MASK  ;

		tmp_val = be32toh(data[1]);
		aun_ir_buf[j] = (tmp_val >> DATA_SHIFT) & DATA_MASK;

		//printf("%d:%d\n", aun_red_buf[j], aun_ir_buf[j]);
	}

	maxim_heart_rate_and_oxygen_saturation(aun_ir_buf, DATA_BUF_SIZE, aun_red_buf, &SpO2, &spo2_valid, &heart_rate, &hr_valid);

	if (spo2_valid)
		printf("SpO2: %d\t", SpO2);
	if (hr_valid)
		printf("hear rate: %d\n", heart_rate);

	while (1) {
		for (i = 100; i < DATA_BUF_SIZE; i++) {
			aun_red_buf[i - 100] = aun_red_buf[i];
			aun_ir_buf[i - 100] = aun_ir_buf[i];
		}

		int count = 0;
		for (j = DATA_BUF_SIZE - 100; j < DATA_BUF_SIZE; j++) {
			struct pollfd pfd = {
				.fd = fd,
				.events = POLLIN,
			};

			ret = poll(&pfd, 1, -1);
			if (ret < 0) {
				ret = -errno;
				goto error;
			} else if (ret == 0) {
				printf("continue\n");
				continue;
			}


			read_size = read(fd, data, 8);
			if (read_size < 0) {
				if (errno == EAGAIN) {
					fprintf(stderr, "nothing available\n");
					continue;
				} else {
					break;
				}
			}
			
			tmp_val = be32toh(data[0]);
			aun_red_buf[j] = (tmp_val >> DATA_SHIFT) & DATA_MASK  ;

			tmp_val = be32toh(data[1]);
			aun_ir_buf[j] = (tmp_val >> DATA_SHIFT) & DATA_MASK;

			count++;
		}
		maxim_heart_rate_and_oxygen_saturation(aun_ir_buf, DATA_BUF_SIZE, aun_red_buf, &SpO2, &spo2_valid, &heart_rate, &hr_valid);

		if (spo2_valid)
			printf("SpO2: %d\t", SpO2);
		if (hr_valid)
			printf("heart rate: %d\n", heart_rate);
	}


error:
	cleanup();
	
	return 0;
}
