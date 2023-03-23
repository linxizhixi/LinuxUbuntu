/*
 * @Author: Zhu Yongtao
 * @Date: 2022-11-25 21:12:19
 * @LastEditTime: 2022-11-25 22:28:37
 */
#include <fcntl.h>
#include <linux/input.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// beeper——fire
#define ESC_KEY 0x1b
#define DEVICE_BEEPER "/dev/input/event0"
#define DEVICE_KEY "/dev/input/event3"

#define SENSOR_MAGIC 'f'
#define F5_READ (_IOR('f', 20, unsigned char))

void enter_adc(char *s);
int read_data(char *file);

int main(int argc, char *argv[])
{
    int fd_beep, fd_button = -1, version, ret;
    struct input_event event, ev;
    struct timeval time;

    if ((fd_beep = open(DEVICE_BEEPER, O_RDWR)) < 0)
    {
        perror("beep test");
        return 1;
    }

    fd_button = open(DEVICE_KEY, O_RDONLY);
    if (fd_button < 0)
    {
        perror("open");
        return -1;
    }

    event.type = EV_SND;
    event.code = SND_TONE;
    event.value = 1000;
    time.tv_sec = 1;
    time.tv_usec = 0;
    event.time = time;
    int flag = 1;

    while (1)
    {
        memset(&ev, 0, sizeof(struct input_event));
        ret = read(fd_button, &ev, sizeof(struct input_event));
        if (ret != sizeof(struct input_event))
        {
            perror("read");
            close(fd_button);
            return -1;
        }
        if (enter_fire_detect())
        {
            if (flag)
            {
                event.value = 1000;
                write(fd_beep, &event, sizeof(struct input_event));
                sleep(1);
                printf("beep is on\n");
                flag = 0;
            }
            else
            {
                event.value = 0;
                write(fd_beep, &event, sizeof(struct input_event));
                sleep(1);
                printf("beep is off\n");
                flag = 1;
            }
        }
    }
    close(fd_button);
    close(fd_button);
}

int enter_fire_detect()
{
    int cmd, i = 10;
    unsigned char arg;

    int fd = open("/dev/sensor-manage", O_RDWR);

    while (1)
    {
        while (i--)
        {
            cmd = F5_READ;
            Ioctl(fd, cmd, &arg);
            if (arg == 0)
            {
                printf("no, no fire\n");
                return 0;
            }
            if (arg == 1)
            {
                printf("Yes, fire!\n");
                return 1;
            }
            sleep(1);
        }
    }
}

void enter_adc(char *s)
{
    int cmd, data, i = 10;
    unsigned char arg;
    char *file;

    file = "/sys/bus/iio/devices/iio:device3/in_voltage6_raw";

    printf("<<<< %s >>>>>\n", s);
    printf("=====>>input 0 to quit\n");
    printf("=====>>input 1 to get sensor value\n");

    while (1)
    {
        i = 10;
        printf("Input options:");
        scanf("%d", &cmd);
        while (getchar() != '\n')
            ;

        if (cmd == 0)
            break;

        switch (cmd)
        {
        case 1:
            while (i--)
            {
                data = read_data(file);
                if (data < 0)
                {
                    perror("read_data");
                    return;
                }
                /*printf("Vol: %0.2fV\n", 1.8 * data / 4096);*/
                printf("data : %05d\n", data);
                sleep(1);
            }
            break;
        default:
            printf("err usage\n");
        }
        cmd = 100;
    }
}

int read_data(char *file)
{
    int fd, ret;
    char buf[10] = {0};
    if ((fd = open(file, O_RDWR)) < 0)
    {
        perror("open");
        return -1;
    }
    ret = read(fd, buf, 10);
    if (ret < 0)
    {
        perror("read");
        return -1;
    }
    close(fd);
    return atoi(buf);
}
