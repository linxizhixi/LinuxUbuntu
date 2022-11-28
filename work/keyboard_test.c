#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <linux/input.h>

#define IOCTL_MAGIC 	'Z'
#define SET_VAL _IO(IOCTL_MAGIC, 0)
#define GET_KEY	_IO(IOCTL_MAGIC, 1)
// 1,开始10秒计时，2,暂停，3，继续，4，清0
// 1,开始，2,暂停，3，继续，4，清0
int timeValue;
unsigned char buf[8] = "0";
int value = 0;
void fun1();
void fun2();
void fun3();
void fun4();

int main(int argc, const char *argv[])
{
	int fd = 0,i = 0;
	int key = 0;

	if ((fd = open("/dev/zlg7290", O_RDWR)) < 0){
		perror("open");
		exit(1);
	}
	while (1){
		ioctl(fd, GET_KEY, &key);
		
		switch (key) {
		case 1: {
			fun1();
			break;
		}
		case 2:{
			fun2();
			break;
		}
		case 3: {
			fun3();
			break;
		}
		case 4: {
			fun4();
			break;
		}
		default: value = 0x00; break;
		}
		//for (i = 0;i < 8;i++)
			//buf[i] = value;
		ioctl(fd, SET_VAL, buf);
	}
	return 0;
}

void fun1(){
	strcpy(buf,"12345678");
}
void fun2(){
	strcpy(buf,"22345678");
}
void fun3(){
	strcpy(buf,"32345678");
	
}
void fun4(){
	strcpy(buf,"42345678");
}

