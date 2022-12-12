#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>	
#include <stdlib.h>

#define SENSOR_MAGIC 'f'
#define SENSOR_MAX_NR 100
#define INIT_BEEP (_IO('f', 8))
#define BEEP_VOICE_UP (_IO('f', 9))
#define BEEP_VOICE_DOWN (_IO('f', 10))
#define EXIT_BEEP (_IO('f', 11))
#define DEVICE_KEY			"/dev/input/event3"

void Ioctl(int fd, unsigned long cmd, void* arg)
{
	if(ioctl(fd, cmd, arg))
		perror("ioctl");
}

int main(void)
{
	int fd = open("/dev/sensor-manage", O_RDWR);
	int fd_button = -1, ret = -1;
	struct input_event ev;
	
	// ��1�������豸�ļ�
	fd_button = open(DEVICE_KEY, O_RDONLY);
	if (fd_button < 0) {
		perror("open");
		return -1;
	}
	
	int cmd;
	unsigned char arg;
	cmd = INIT_BEEP;
	Ioctl(fd, cmd, &arg);
	
	while (1) {
		// ��2������ȡһ��event�¼���
		memset(&ev, 0, sizeof(struct input_event));
		ret = read(fd_button, &ev, sizeof(struct input_event));
		if (ret != sizeof(struct input_event))
		{
			perror("read");
			close(fd_button);
			return -1;
		}
		
		// ��3��������event������֪��������ʲô���������¼�
		printf("-------------------------\n");
		printf("type: %hd\n", ev.type);
		printf("code: %hd\n", ev.code);
		printf("value: %d\n", ev.value);
		printf("\n");
		
		int flag=1;
		if(ev.value){
			if(flag){
				cmd = BEEP_VOICE_UP;
				Ioctl(fd, cmd, &arg);
				printf( "beep is on\n");
				flag=0;
			}
			else{
			cmd = BEEP_VOICE_UP;
			Ioctl(fd, cmd, &arg);
			printf( "beep is off\n");
			flag=1;
			}
		}
	}
	
	
	// ��4�����ر��豸
	close(fd_button);
	
	return 0;
}







