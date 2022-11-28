#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iconv.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <fcntl.h>

#include "led1616.h"

#define key_event_file "/dev/input/event3"

int main(int argc, char **argv)
{
    int key_fd = -1;
    struct input_event key_ev;
    unsigned char show_str[][4] = {"A", "B", "C", "D"};
    int show_num = sizeof(show_str)/sizeof(show_str[0]);
    int index = 0;

    key_fd = open(key_event_file, O_RDONLY);
    display(show_str[index],strlen(show_str[index]));

    while (1)
    {
        memset(&key_ev, 0, sizeof(key_ev));
        read(key_fd, &key_ev, sizeof(key_ev));

        if (key_ev.code == 116 && key_ev.type == 1)
        {
            if (key_ev.value == 1)
            {
                if (index == 3)
                {
                    index = 0;
                }
                else
                {
                    index++;
                }
                display(show_str[index],strlen(show_str[index]));
            }
        }
    }
}
