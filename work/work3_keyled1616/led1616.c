/*
 * @Author: Zhu Yongtao
 * @Date: 2022-11-28 19:38:40
 * @LastEditTime: 2022-11-28 20:26:53
 */
#include "led1616.h"

unsigned int decode(unsigned char *code)
{
    unsigned char part;
    unsigned char addr;
    unsigned short realaddr, ttp, temp;
    unsigned int i_addr;

    if (code[0] < 0xA0)
    {
        part = 0x2;
        addr = code[0] - 33;
    }
    else
    {
        part = code[0] - 0xA1;
        addr = code[1] - 0xA1;
        part -= 6;
    }

    if (!part && !addr)
        return -1;

    i_addr = addr * 32 + part * 94 * 32;

    return i_addr;
}

void display(unsigned char *string, unsigned int size)
{
    unsigned int offset;
    FILE *file;
    int fd;
    struct hz hanzi;
    int i = 0;

    fd = open("/dev/led1616", O_RDWR);
    if (fd < 0)
    {
        perror("open");
        exit(1);
    }

    file = fopen("/lib/cclib", "rb");
    if (file == NULL)
    {
        perror("fopen");
        exit(1);
    }

    while (i < size)
    {
        offset = decode(string + i);

        fseek(file, offset, 0);

        fread(hanzi.code, sizeof(char), 32, file);

        ioctl(fd, LED_DISPLAY, &hanzi);

        if (string[i] < 0xA0)
        {
            i += 1;
        }
        else
        {
            i += 2;
        }
    }

    fclose(file);
    close(fd);
}
