#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <string.h>

#define SET_VAL _IO('R', 0)

int main(int argc, const char *argv[])
{
	
	uint8_t card_data[32] = {0};

	int i = 0;
	int nbyte = 0;

	int fd = open("/dev/rfid_module0", O_RDWR);
	if (fd < 0) {
		perror("open");
		exit(1);
	}

	while(1) {

		nbyte = read(fd,card_data,4);
		if(nbyte < 0)
		{
			perror("read");
		}

		printf("card data:");
		for(i=0;i<4;i++)
		{
			printf("%02x ",card_data[i]);
		}
		printf("\n");

		memset(card_data,0,sizeof(card_data));
		sleep(1);
		
	}

	return 0;
}

