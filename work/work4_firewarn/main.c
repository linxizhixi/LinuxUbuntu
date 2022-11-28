#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <linux/input.h>
#include "cmd_for_sensor.h"

#define beep_event_file "/dev/input/event0"

int main(int argc, char *argv[])
{
    int sensor_fd;
    int beep_fd;
    unsigned char arg;
    struct input_event beep_ev;
    struct timeval time;

    sensor_fd = open("/dev/sensor-manage", O_RDWR);
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
        ioctl(sensor_fd, F5_READ, &arg);
        if (arg == 0)
        {
            printf("no fire\n");
            beep_ev.value = 0;
            write(beep_fd, &beep_ev, sizeof(beep_ev));
        }

        if (arg == 1)
        {
            printf("fire!\n");
            beep_ev.value = 10000;
            write(beep_fd, &beep_ev, sizeof(beep_ev));
        }

        usleep(100000);
    }

    return 0;
}