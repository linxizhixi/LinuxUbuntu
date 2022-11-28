#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/input.h>
#include <unistd.h>

#define ESC_KEY  0x1b
#define DEVICE_BEEPER		"/dev/input/event0"

int main(int argc, char *argv[])
{
	int fd_beep, version, ret;
	struct input_event event;
	struct timeval time;

	if (argc != 2) {
		printf("usage: %s <device path>\n", argv[0]);
		return 1;
	}

	if ((fd_beep = open(DEVICE_BEEPER, O_RDWR)) < 0) {
		perror("beep test");
		return 1;
	}

	event.type = EV_SND;
	event.code = SND_TONE;
	event.value = 1000;
	time.tv_sec = 1;
	time.tv_usec = 0;
	event.time = time;

	while( 1 )
	{
		int key;

		ret = write(fd_beep, &event, sizeof(struct input_event));

		printf( "Freq = %d\n", event.value);

		key = getchar();

		switch(key) {
		case '+':
			if( event.value < 20000)
				event.value += 10;
			break;

		case '-':
			if( event.value > 11 )
				event.value -= 10 ;
			break;

		case ESC_KEY:
		case EOF:   
			event.code = SND_BELL;
			event.value = 0000;     
			ret = write(fd_beep, &event, sizeof(struct input_event));
			close(fd_beep);
			return 1;
		default:
			break;
		}
	}
}
