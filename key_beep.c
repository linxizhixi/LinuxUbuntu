#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>
#include <string.h>
#include <unistd.h>

#define DEVICE_KEY			"/dev/input/event3"
#define DEVICE_BEEP			"/dev/input/event0"

int main(void)
{
	int fd = -1, ret = -1;
	struct input_event ev;

	int fd2=-1, version;
	struct input_event event;
	struct timeval time;



	// 第1步：打开设备文件
	fd = open(DEVICE_KEY, O_RDONLY);
	fd2 = open(DEVICE_BEEP,O_RDWR);
	if (fd < 0||fd2 < 0) {
		perror("open");
		return -1;
	}
      event.type = EV_SND;
	event.code = SND_TONE;
	event.value = 0;
	time.tv_sec = 1;
	time.tv_usec = 0;
	event.time = time;

	while (1) {
		// 第2步：读取一个event事件包
		memset(&ev, 0, sizeof(struct input_event));
		ret = read(fd, &ev, sizeof(struct input_event));

		if (ret != sizeof(struct input_event))
		{
			perror("read");
			close(fd);
			return -1;
		}
		sleep(1);

		// 第3步：解析event包，才知道发生了什么样的输入事件
        printf("-------------------------\n");
		printf("type: %hd\n", ev.type);
		printf("code: %hd\n", ev.code);
		printf("value: %d\n", ev.value);
		printf("\n");

		if(ev.value==1)
		{
				event.value = 1000;
				ret = write(fd2, &event, sizeof(struct input_event));
		}

		if(ev.value==0)
		{
			event.value = 0000;
			ret = write(fd2, &event, sizeof(struct input_event));
		}

	}

	// 第4步：关闭设备
	close(fd);
	close(fd2);
	return 0;
}

