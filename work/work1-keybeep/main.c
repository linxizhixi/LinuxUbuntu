
#include <fcntl.h>
#include <linux/input.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define key_event_file "/dev/input/event3"
#define beep_event_file "/dev/input/event0"

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

int main(int argc, char **argv)
{
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