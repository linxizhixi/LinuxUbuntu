/*
 * @Author: ZhuYongtao
 * @Date: 2022-12-05 00:40:18
 * @LastEditTime: 2022-12-05 01:35:43
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <linux/input.h>

#define IOCTL_MAGIC 	'Z'
#define SET_VAL _IO(IOCTL_MAGIC, 0)
#define GET_KEY	_IO(IOCTL_MAGIC, 1)
FILE* file_led ="/sys/class/leds/user1/brightness";

int main(int argc, const char *argv[])
{
	int fd = 0,i = 0;
	unsigned char buf[8] = "0";
	int value = 0;
	int key = 0;

	if ((fd = open("/dev/zlg7290", O_RDWR)) < 0){
		perror("open");
		exit(1);
	}
	while (1){
		ioctl(fd, GET_KEY, &key);

		switch (key) {
		case 1:	
			value = '1'; 
			char *buf = "1";
			// freopen("/sys/class/leds/user1/brightness", "w", buf);
			freopen(buf, "r",file_led) ;
			break;
		case 2: 
			value = '2'; 
			strcpy(buf, "0");
			// freopen("/sys/class/leds/user1/brightness", "w", buf);
			freopen(buf, "r", file_led);
			break;
		case 3: 
			value = '3'; 
			break;
		case 4: 
			value = '4'; 
			break;
		case 9: 
			value = '5'; 
			break;
		default: value = 0x00; break;
		}
		for (i = 0;i < 8;i++)
			buf[i] = value;
		ioctl(fd, SET_VAL, buf);
	}
	return 0;
}

