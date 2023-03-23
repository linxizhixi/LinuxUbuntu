/*
 * @Author: ZhuYongtao
 * @Date: 2022-12-09 00:49:53
 * @LastEditTime: 2023-03-23 12:39:20
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iconv.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "led1616.h"

#define OUTLEN 255
unsigned int decode(unsigned char *);
void display(unsigned char *, unsigned  int);

// int main(int argc, char *argv[])
// {
// 	// char *input = "����һ��LED������ʾʾ��ABCDabcd#!@#%^&";
// 	char *input = "������3200515046";
// 	size_t len = strlen(input);
// 	while(1){
// 		display(input, len);
// 		sleep(2);
// 	}
// 	return 0;
// }

unsigned int decode(unsigned char *code)
{
	unsigned char part;
	unsigned char addr;
	unsigned short realaddr,ttp,temp;
	unsigned int i_addr;

	if (code[0] < 0xA0) {
		part = 0x2;
		addr = code[0] - 33;
	} else {
		part = code[0] - 0xA1;
		addr = code[1] - 0xA1;
		part -= 6;
	}
	printf("part = %d, addr = %d\n", part, addr);

	if (!part && !addr)
		return;

	i_addr = addr * 32 + part * 94 * 32;

	printf("part = %d, addr = %d, i_addr = %d\n", part, addr, i_addr);

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
	if (fd < 0) {
		perror("open");
		exit(1);
	}

	file = fopen("/lib/cclib", "rb");
	if (file == NULL) {
		perror("fopen");
		exit(1);
	}

	while(i < size) {

		offset = decode(string + i);
		
		fseek(file, offset, 0);

		fread(hanzi.code, sizeof(char), 32, file);

		ioctl(fd, LED_DISPLAY, &hanzi);
		
		if (string[i] < 0xA0) {
			i += 1;
		} else {
			i += 2;
		}

		sleep(1);
	}

	fclose(file);
	close(fd);
}
