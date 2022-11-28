#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>


int read_sysfs_float(const char *device, const char *filename, float *val)
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
	if (fscanf(sysfsfp, "%f\n", val) != 1) {
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
	int temp_raw = 0;
	int temp_offset = 0;
	float temp_scale = 0;

	int hum_raw = 0;
	int hum_offset = 0;
	float hum_scale = 0;

	if (argc != 2) {
		printf("usage: %s <iio:device[n]>\n", argv[0]);
		return 1;
	}

	while(1) {
		/*read temp data*/
		read_sysfs_int(argv[1], "in_temp_raw", &temp_raw);
		read_sysfs_int(argv[1], "in_temp_offset", &temp_offset);
		read_sysfs_float(argv[1], "in_temp_scale", &temp_scale);

		printf("temperature = %.2f\n", (temp_raw + temp_offset) * temp_scale / 1000);

		read_sysfs_int(argv[1], "in_humidityrelative_raw", &hum_raw);
		read_sysfs_int(argv[1], "in_humidityrelative_offset", &hum_offset);
		read_sysfs_float(argv[1], "in_humidityrelative_scale", &hum_scale);

		printf("humidity = %.2f%%\n", (hum_raw + hum_offset) * hum_scale / 1000);
		
		sleep(1);
	}

	return 0;
}
