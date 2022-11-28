#ifndef LED1616_H
#define LED1616_H

#define LED_MAGIC 'L'

struct hz
{
	unsigned char code[33];
};

#define LED_DISPLAY _IOW(LED_MAGIC, 0, struct hz)

#endif
