#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define R_1K  1000
#define R_100K (100 * R_1K)
#define R_10  10 
#define R_0D1 0.1

int read_sysfs_int(const char *device, const char *filename, int *val)
{
	int ret = 0;
	FILE  *sysfsfp;
	char temp[128]; 

	memset(temp, '0', 128);

	ret = sprintf(temp, "/sys/bus/iio/devices/%s/%s", device, filename);
	if (ret < 0)
		goto error;

	sysfsfp = fopen(temp, "r");
	if (!sysfsfp) {
		ret = -errno;
		goto error;
	}

	errno = 0;
	if (fscanf(sysfsfp, "%d\n", val) != 1) {
		ret = errno ? -errno : -ENODATA;
		if (fclose(sysfsfp))
			perror("read_sysfs_float(): Failed to close dir");

		goto error;
	}

	if (fclose(sysfsfp))
		ret = -errno;

error:

	return ret;
}

int main(int argc, char *argv[])
{
	int data = 0;

	if (argc != 2) {
		printf("usage: %s <iio:device[n]>\n", argv[0]);
		return 1;
	}

	while(1) {
		read_sysfs_int(argv[1], "in_voltage0_raw", &data);
		if (data < 0) {
			return -1;
		}
		printf("current value = %0.2f mA\n", ((((float)3300 * data) \
						/ 65536)/ (R_10 + R_100K + R_1K)) * R_1K / R_0D1);

		read_sysfs_int(argv[1], "in_voltage1_raw", &data);
		if (data < 0) {
			return -1;
		}
		printf("voltage value = %0.2f mV\n", (float)3300 * data / 65536);

		sleep(2);
	}

	return 0;
}
