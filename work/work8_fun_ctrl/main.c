/*
 * @Author: Zhu Yongtao
 * @Date: 2022-11-28 19:38:40
 * @LastEditTime: 2022-12-04 23:01:16
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

int main(char **argv, int argc)
{
    unsigned char arg;
    int cmd;
    int fd = open("/dev/sensor-manage", O_RDWR);
    if (fd < 0)
    {
        perror("open");
        return -1;
    }

    ioctl(fd, INIT_FAN, &arg);

    while (1)
    {
        unsigned char infra_received;
        ioctl(fd, D1_READ, &infra_received);
        printf("infra_received:");
        printf("%d\n", &infra_received);

        switch (infra_received)
        {
        case 0:
            cmd = FAN_DOWN;
            ioctl(fd, cmd, &arg);
            break;
        case 1:
            cmd = FAN_UP;
            ioctl(fd, cmd, &arg);
            break;
        case 2:
            cmd = EXIT_FAN;
            ioctl(fd, cmd, &arg);
            break;
        default:
            ;
        }
        cmd = 100;
        usleep(100000);
    }
    cmd = EXIT_FAN;
    ioctl(fd, cmd, &arg);
}
