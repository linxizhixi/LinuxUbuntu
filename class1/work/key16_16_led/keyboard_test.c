#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <linux/input.h>

#define IOCTL_MAGIC 	'Z'
#define SET_VAL _IO(IOCTL_MAGIC, 0)
#define GET_KEY	_IO(IOCTL_MAGIC, 1)

#define SENSOR_MAGIC 'f'
#define SENSOR_MAX_NR 100
#define INIT_BEEP (_IO('f', 8))
#define BEEP_VOICE_UP (_IO('f', 9))
#define BEEP_VOICE_DOWN (_IO('f', 10))
#define EXIT_BEEP (_IO('f', 11))
#define DEVICE_KEY			"/dev/input/event3"

int main(int argc, const char *argv[])
{
	int fd = 0,fd_sensor=0,i = 0;
	unsigned char buf[8] = "0";
	int value = 0;
	int key = 0;

	if ((fd = open("/dev/zlg7290", O_RDWR)) < 0){
		perror("open");
		exit(1);
	}
	
	if ((fd_sensor = open("/dev/sensor-manage", O_RDWR)) < 0){
		perror("open");
		exit(1);
	}
	
	int cmd;
	unsigned char arg;
	cmd = INIT_BEEP;
	ioctl(fd_sensor, cmd, &arg);
	
	while (1){
		ioctl(fd, GET_KEY, &key);
		
		switch (key) {
		case 1:
			cmd = BEEP_VOICE_UP;
			ioctl(fd_sensor, cmd, &arg);
			value = '1'; 
			break;
		case 2: 
			cmd = BEEP_VOICE_DOWN;
			ioctl(fd_sensor, cmd, &arg);
			value = '2'; 
			break;
		case 3: 
			cmd = EXIT_BEEP;
			ioctl(fd_sensor, cmd, &arg);
			value = '3'; 
			break;
		case 4: 
			value = '4'; 
			break;
		default: 
			value = 0x00;
			break;
		}
		for (i = 0;i < 8;i++)
			buf[i] = value;
		ioctl(fd, SET_VAL, buf);
	}
	return 0;
}

