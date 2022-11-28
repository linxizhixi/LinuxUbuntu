#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>
#include <string.h>
#include <unistd.h>


#define DEVICE_KEY			"/dev/input/event3"
#define DEVICE_BEEPER		"/dev/input/event0"

int main(void)
{
	int fd_key = -1, ret = -1,fd_beep;
	struct input_event ev;
	struct timeval time;
	
	// ��1�������豸�ļ�
	fd_key = open(DEVICE_KEY, O_RDONLY);
	if (fd_key < 0) {
		perror("open");
		return -1;
	}
	if ((fd_beep = open(DEVICE_BEEPER, O_RDWR)) < 0) {
		perror("beep test");
		return 1;
	}
	
	ev.type = EV_SND;
	ev.code = SND_TONE;
	ev.value = 1000;
	time.tv_sec = 1;
	time.tv_usec = 0;
	ev.time = time;
	
	while (1) {
		// ��2������ȡһ��event�¼���
		memset(&ev, 0, sizeof(struct input_event));
		ret = read(fd_key, &ev, sizeof(struct input_event));
		if (ret != sizeof(struct input_event))
		{
			perror("read");
			close(fd_key);
			return -1;
		}
		
		// ��3��������event������֪��������ʲô���������¼�
		printf("-------------------------\n");
		printf("type: %hd\n", ev.type);
		printf("code: %hd\n", ev.code);
		printf("value: %d\n", ev.value);
		printf("\n");
		
		
		write(fd_beep, &ev, sizeof(struct input_event));
		
	}
	
	
	// ��4�����ر��豸
	close(fd_key);
	close(fd_beep);
	return 0;
}







