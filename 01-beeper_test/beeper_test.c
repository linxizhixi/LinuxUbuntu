#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/input.h>
#include <unistd.h>
// beeper——kay
#define ESC_KEY  0x1b
#define DEVICE_BEEPER		"/dev/input/event0"
#define DEVICE_KEY			"/dev/input/event3"

int main(int argc, char *argv[])
{
	int fd_beep,fd_button = -1, version, ret;
	struct input_event event,ev;
	struct timeval time;

	if ((fd_beep = open(DEVICE_BEEPER, O_RDWR)) < 0) {
		perror("beep test");
		return 1;
	}
	
		// 第1步：打开设备文件
	fd_button = open(DEVICE_KEY, O_RDONLY);
	if (fd_button < 0) {
		perror("open");
		return -1;
	}

	event.type = EV_SND;
	event.code = SND_TONE;
	event.value = 1000;
	time.tv_sec = 1;
	time.tv_usec = 0;
	event.time = time;
	int flag=1;

	while( 1 )
	{
		// 第2步：读取一个event事件包
		memset(&ev, 0, sizeof(struct input_event));
		ret = read(fd_button, &ev, sizeof(struct input_event));
		if (ret != sizeof(struct input_event))
		{
			perror("read");
			close(fd_button);
			return -1;
		}
		if(ev.value){
			if(flag){
				event.value=1000;
				write(fd_beep, &event, sizeof(struct input_event));
				sleep(1);
				printf( "beep is on\n");
				flag=0;
			}
			else{
			event.value=0;
			write(fd_beep, &event, sizeof(struct input_event));
			sleep(1);
			printf( "beep is off\n");
			flag=1;
			}
		}
		
	}
	// 第4步：关闭设备
	close(fd_button);
	close(fd_button);
}
