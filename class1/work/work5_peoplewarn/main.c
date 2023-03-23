/*
 * @Author: Dafu Wang
 * @Date: 2022-11-21 16:52:14
 * @LastEditTime: 2022-11-28 14:17:24
 * @LastEditors: Dafu Wang
 * @FilePath: \driver_sample\work5_peoplewarn\main.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/input.h>
#include "cmd_for_sensor.h"

#define beep_event_file "/dev/input/event0"
#define people_ir_get "gpioget gpiochip7 6"

int main(int argc, char *argv[])
{
    int beep_fd;
    unsigned char arg;
    struct input_event beep_ev;
    struct timeval time;
    FILE *gpio_file;
    char gpio_buf[10];

    beep_fd = open(beep_event_file, O_RDWR);
    beep_ev.type = EV_SND;
    beep_ev.code = SND_TONE;
    beep_ev.value = 0;
    time.tv_sec = 1;
    time.tv_usec = 0;
    beep_ev.time = time;
    write(beep_fd, &beep_ev, sizeof(beep_ev));

    while (1)
    {
        gpio_file=popen(people_ir_get,"r");
        fread(gpio_buf,sizeof(gpio_buf),1,gpio_file);
        pclose(gpio_file);
        if(strcmp(gpio_buf,"1\n")==0)
        {
            printf("people!!!\n");
            beep_ev.value = 10000;
            write(beep_fd, &beep_ev, sizeof(beep_ev));
        }
        else
        {
            beep_ev.value = 0;
            write(beep_fd, &beep_ev, sizeof(beep_ev));
        }
        usleep(100000);
    }
    pclose(gpio_file);

    return 0;
}