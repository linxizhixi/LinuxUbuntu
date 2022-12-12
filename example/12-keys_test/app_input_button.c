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
	
	// 第1步：打开设备文件
	fd_button = open(DEVICE_KEY, O_RDONLY);
	if (fd_button < 0) {
		perror("open");
		return -1;
	}
	
	while (1) {
		// 第2步：读取一个event事件包
		memset(&ev, 0, sizeof(struct input_event));
		ret = read(fd_button, &ev, sizeof(struct input_event));
		if (ret != sizeof(struct input_event))
		{
			perror("read");
			close(fd_button);
			return -1;
		}
		
		// 第3步：解析event包，才知道发生了什么样的输入事件
		printf("-------------------------\n");
		printf("type: %hd\n", ev.type);
		printf("code: %hd\n", ev.code);
		printf("value: %d\n", ev.value);
		printf("\n");
	}
	
	
	// 第4步：关闭设备
	close(fd_button);
	
	return 0;
}







