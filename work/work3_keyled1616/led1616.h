#ifndef LED1616_H
#define LED1616_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iconv.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>


#define LED_MAGIC 'L'

struct hz
{
	unsigned char code[33];
};

void display(unsigned char *, unsigned  int);

#define LED_DISPLAY _IOW(LED_MAGIC, 0, struct hz)

#endif
