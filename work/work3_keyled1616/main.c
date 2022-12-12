/*
 * @Author: ZhuYongtao
 * @Date: 2022-12-04 21:04:45
 * @LastEditTime: 2022-12-09 00:26:47
 */
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

// #define CHAR_MAX    6
#define CHAR_MAX    13
unsigned char show_str[][CHAR_MAX] = {"朱","永","涛", "3","2","0", "0","5","1","5","0","4","6"};
int main(int argc, char **argv)
{
    int key_fd = -1;
    struct input_event key_ev;
    // unsigned char show_str[][CHAR_MAX] = {"Z", "Y", "T", "0","4","6"};
    int show_num = sizeof(show_str)/sizeof(show_str[0]);
    int index = 0;

    key_fd = open(key_event_file, O_RDONLY);
    display(show_str[index],strlen(show_str[index]));

    while (1)
    {
        memset(&key_ev, 0, sizeof(key_ev));
        read(key_fd, &key_ev, sizeof(key_ev));

        // if (key_ev.code == 116 && key_ev.type == 1)
        // {
        //     if (key_ev.value == 1)
        //     {
        //         if (index == CHAR_MAX)
        //         {
        //             index = 0;
        //         }
        //         else
        //         {
        //             index++;
        //         }
        //         display(show_str[index],strlen(show_str[index]));
        //     }
        // }

        if (index == CHAR_MAX)
        {
            index = 0;
        }
        else
        {
            index++;
        }
        display(show_str[index],strlen(show_str[index]));
        sleep(100);

    }
}
