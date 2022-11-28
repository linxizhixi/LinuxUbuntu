#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <iconv.h>
#include <string.h>

#define BUF_LEN 128

void serial_init(int fd)
{
	struct termios options;
	tcgetattr(fd,&options);
	options.c_cflag |=(CLOCAL | CREAD);
	options.c_cflag &= ~CSIZE;
	options.c_cflag &= ~CRTSCTS;
	options.c_cflag |= CS8;
	options.c_cflag &= ~CSTOPB;
	options.c_iflag |= IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;

	//if((cfsetispeed(&options,B19200)) == -1)
	if((cfsetispeed(&options,B115200)) == -1)
	{
		printf("cfsetospeed is erro\n");
		exit(-1);
	}

	//cfsetospeed(&options,B115200);
	tcsetattr(fd,TCSANOW,&options);

}

int main(int argc, const char *argv[])
{
	int fd, nbytes, i;
	char buf[BUF_LEN] = "hello world\n";

	if((fd = open(argv[2], O_RDWR)) < 0)
	{
		perror("fail to open serial port");
		exit(-1);
	}

	serial_init(fd);

	printf("RS485 Test: Open Device OK!\n");


	printf("argv[1] = %s, argv[2] = %s\n", argv[1], argv[2]);
/****************  read data  ********************/

	printf("--------------\n");
	if(argv[1][1] == 'r')
	{
		printf("\nWaiting Data ...\n");

		while (1)
		{
			memset(buf, 0, BUF_LEN);

			nbytes = read(fd, buf, sizeof(buf));

			printf("%s\n", buf);
		}
	}

/****************  read data  ********************/
	printf("--------------\n");

	if(argv[1][1] == 's')
	{
		while (1)
		{
			nbytes = write(fd, buf, strlen(buf));
		}
	}

	close(fd);

	return 0;
}

