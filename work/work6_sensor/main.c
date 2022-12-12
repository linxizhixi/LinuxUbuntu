
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include "cmd_for_sensor.h"

int main(char **argv, int argc)
{
	int cmd, i,fd,data,adc_fd;
	struct dht11_data arg;
	char buf[10] = {0};

	fd = open("/dev/sensor-manage", O_RDWR);

	while (1)
	{
		cmd = READ_DHT11;
		ioctl(fd, cmd, &arg);

		adc_fd=open("/sys/bus/iio/devices/iio:device3/in_voltage6_raw",O_RDWR);
		read(adc_fd, buf, 10);
		close(adc_fd);

		data = atoi(buf);
		printf("\nhumi : %02hhd.%02hhd%%\ntemp: %02hhd.%02hhd°C\nlight:%05d ", arg.hu_h, arg.hu_l, arg.te_h, arg.te_l,data);
		sleep(2);
	}
}
