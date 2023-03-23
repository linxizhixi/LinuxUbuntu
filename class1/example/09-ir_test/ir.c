#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <linux/input.h>

typedef struct key {
	unsigned char key;
	char key_desc[30];
}key1_t;

#define BRIEF_LINES 4
char *brief[BRIEF_LINES] = {
" =================================",
"| 实验：红外测试                   |",
"| 现象：按下遥控按键，显示对应键值 |",  
" ================================="};

key1_t table[] = {
 {KEY_1, "key 1"},
 {KEY_2, "key 2"},
 {KEY_3, "key 3"},
 {KEY_4, "key 4"},
 {KEY_5, "key 5"},
 {KEY_6, "key 6"},
 {KEY_7, "key 7"},
 {KEY_8, "key 8"},
 {KEY_9, "key 9"},
 {KEY_SETUP, "key setup"},
 {KEY_HOME, "key home"},
 {KEY_BACK, "key back"},
 {KEY_MENU, "key menu"},
};

void draw_string(char *buf[], unsigned int line)
{
	int i;
	for (i = 0; i < line; i++) {
		printf("%s\n", buf[i]);
	}
}

int main(int argc, const char *argv[])
{
	struct input_event ev;
	int i = 0;
	int fd = open("/dev/input/event3", O_RDWR);
	if (fd < 0) {
		perror("open");
		exit(1);
	}
	draw_string(brief, BRIEF_LINES);
	while(1) {
		read(fd, &ev, sizeof(ev));
//		printf("ev.code = %d\n", ev.code);
#if 1
		switch(ev.type)
		{
		case EV_KEY:
			for (i = 0; i < (sizeof(table) / sizeof(table[0])); i++) {
				if (table[i].key == ev.code) {
					if (ev.value == 1) 
						printf("%s is press\n", table[i].key_desc);
					else
						printf("%s is release\n", table[i].key_desc);
					break;
				}	
			}
		default:
			break;
		}
#endif
	}

	return 0;
}

