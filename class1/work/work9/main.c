/*
 * @Author: ZhuYongtao
 * @Date: 2022-12-09 00:16:24
 * @LastEditTime: 2022-12-11 22:47:38
 */

#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "led1616.h"

#define key_event_file "/dev/input/event3"
#define beep_event_file "/dev/input/event0"

#define IOCTL_MAGIC 	'Z'
#define SET_VAL _IO(IOCTL_MAGIC, 0)
#define GET_KEY	_IO(IOCTL_MAGIC, 1)

void display(unsigned char *, unsigned  int);

int beep_fd;
struct input_event beep_ev;
struct timeval time;

int key_fd;
struct input_event key_ev;

void beep_init(void)
{
    beep_fd = open(beep_event_file, O_RDWR);
    beep_ev.type = EV_SND;
    beep_ev.code = SND_TONE;
    beep_ev.value = 0;
    time.tv_sec = 1;
    time.tv_usec = 0;
    beep_ev.time = time;
    write(beep_fd, &beep_ev, sizeof(beep_ev));
}

void beep_on(void)
{
    beep_ev.value = 10000;
    write(beep_fd, &beep_ev, sizeof(beep_ev));
}

void beep_off(void)
{
    beep_ev.value = 0;
    write(beep_fd, &beep_ev, sizeof(beep_ev));
}

void key_init(void)
{
    key_fd = open(key_event_file, O_RDONLY);
}

/**
 * @brief
 *
 * @return int 1按键按下 0按键抬起 -1按键空闲
 */
int key_scan(void)
{
    memset(&key_ev, 0, sizeof(key_ev));
    read(key_fd, &key_ev, sizeof(key_ev));

    if (key_ev.code == 116 && key_ev.type == 1)
    {
        if (key_ev.value == 1)
        {
            return 1;
        }
        else if (key_ev.value == 0)
        {
            return 0;
        }
    }
    else
    {
        return -1;
    }
}

void key_beep(){
    int key_val = 0;
    key_init();
    beep_init();

    while (1)
    {
        key_val = key_scan();
        if (key_scan() == 1)
        {
            beep_on();
        }
        else if (key_scan() == 0)
        {
            beep_off();
        }
        usleep(100000);
    }
}

void show_name(){
	char *input = "zyt3200515046";
	size_t len = strlen(input);
	while(1){
		display(input, len);
		sleep(2);
	}
}

void key16(){
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
			// char *buf = "1";
			// freopen("/sys/class/leds/user1/brightness", "w", buf);
			// freopen(buf, "r",file_led) ;
			break;
		case 2: 
			value = '2'; 
			// strcpy(buf, "0");
			// freopen("/sys/class/leds/user1/brightness", "w", buf);
			// freopen(buf, "r", file_led);
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
}

int main(int argc, char **argv)
{
    int i,pid1, pid2,pid3;
    while ((pid1 = fork()) == -1);
    if (pid1 == 0)//son
    {
        while ((pid2 = fork())==-1);
        if (pid2 == 0){
            show_name();
        }
        else{
            key_beep();
        }
    }
    else {
        while ((pid3 = fork())==-1);
        if (pid3 == 0){
            key16();
        }
        else{
            
        }
    }
}