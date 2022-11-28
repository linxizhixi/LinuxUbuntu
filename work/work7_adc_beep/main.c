/*
 * @Author: Zhu Yongtao
 * @Date: 2022-11-28 19:38:40
 * @LastEditTime: 2022-11-28 20:48:52
 */
#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "cmd_for_sensor.h"

#define beep_event_file "/dev/input/event0"
#define adc_file "/sys/bus/iio/devices/iio:device3/in_voltage1_raw"

struct input_event beep_ev;
struct timeval time;
int beep_fd;

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

int read_adc(void)
{
    FILE *adc_fd;
    char buf[10] = {0};
    adc_fd = fopen(adc_file, "r");
    fscanf(adc_fd, "%s\n", buf);
    fclose(adc_fd);
    return atoi(buf);
}

int main(char **argv, int argc)
{
    int adc_vol = 0;

    beep_init();

    while (1)
    {
        adc_vol = read_adc();
        printf("%d\n", adc_vol);
        if (adc_vol < 1200)
        {
            beep_on();
        }
        else
        {
            beep_off();
        }
        usleep(500000);
    }
}
