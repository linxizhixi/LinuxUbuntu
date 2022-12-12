#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>
#include <string.h>
#include <unistd.h>


#define DEVICE_KEY			"/dev/input/event3"

int main(void)
{
	int fd_button = -1, ret = -1;
	struct input_event ev;
	
	// ��1�������豸�ļ�
	fd_button = open(DEVICE_KEY, O_RDONLY);
	if (fd_button < 0) {
		perror("open");
		return -1;
	}
	
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
	}
	
	
	// ��4�����ر��豸
	close(fd_button);
	
	return 0;
}







