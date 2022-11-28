#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include "cmd_for_sensor.h"


void Ioctl(int fd, unsigned long cmd, void* arg)
{
	if(ioctl(fd, cmd, arg))
		perror("ioctl");
}

void enter_fire_detect(int fd)
{
	int cmd, i=10;
	unsigned char arg;


	printf("<<<<fire detect>>>>>\n");
	printf("=====>>input 0 to quit\n");
	printf("=====>>input 1 to get status\n");

	while (1) {
		i = 10;
		printf("Input fire detect options:");
		scanf("%d", &cmd);
		while(getchar() != '\n');

		if (cmd == 0) 
			break;
		switch (cmd) {
			case 1:
				while (i--) 
				{
					cmd = F5_READ;	
					//#define F5_READ ( _IOR('f', 20, unsigned char))
					Ioctl(fd, cmd, &arg);
					if (arg == 0)
						printf("no, no fire\n");
					if (arg == 1)
						printf("Yes, fire!\n");
					sleep(1);
				}
				break;
			default:
				printf("err usage\n");
		}
	}
}

void set_fan_speed(int fd)
{
	int cmd, arg;
	char ch;
	printf("===>>Please input + or - to set fan speed\n");
	printf("===>> input q to quit\n");

	while (1) {
		printf("Input fan speed:");
		scanf("%c", &ch);
		while(getchar() != '\n');		

		if(ch == 'q')
			break;

		switch (ch) {
			case '+':
				cmd = FAN_UP;
				Ioctl(fd, cmd, &arg);
				break;
			case '-':
				cmd = FAN_DOWN;
				Ioctl(fd, cmd, &arg);
				break;
			default:
				printf("err usage\n");
		}
		ch = 'a';
	}
}

void enter_fan(int fd)
{
	int cmd;
	unsigned char arg;

	cmd = INIT_FAN;
	Ioctl(fd, cmd, &arg);

	printf("<<<< fan >>>>>\n");
	printf("=====>>input 0 to quit\n");
	printf("=====>>input 1 to set fan speed\n");

	while (1) {
		printf("Input fan options:");
		scanf("%d", &cmd);
		while(getchar() != '\n');

		if (cmd == 0) {
			cmd = EXIT_FAN;
			Ioctl(fd, cmd, &arg);
			break;
		}
			
		switch (cmd) {
			case 1:
				set_fan_speed(fd);
				break;
			default:
				printf("err usage\n");
		}
		cmd = 100;
	}

}

void set_beep_voice(int fd)
{
	int cmd, arg;
	char ch;
	printf("===>>Please input + or - to set beep voice\n");
	printf("===>> input q to quit\n");

	while (1) {
		printf("Input beep voice:");
		scanf("%c", &ch);
		while(getchar() != '\n');		

		if(ch == 'q')
			break;

		switch (ch) {
			case '+':
				cmd = BEEP_VOICE_UP;
				Ioctl(fd, cmd, &arg);
				break;
			case '-':
				cmd = BEEP_VOICE_DOWN;
				Ioctl(fd, cmd, &arg);
				break;
			default:
				printf("err usage\n");
		}
		ch = 'a';
	}

}

void enter_beep(int fd)
{
	int cmd;
	unsigned char arg;

	cmd = INIT_BEEP;
	Ioctl(fd, cmd, &arg);

	printf("<<<< beep >>>>>\n");
	printf("=====>>input 0 to quit\n");
	printf("=====>>input 1 to set beep voice\n");
	printf("=====>>input 2 to play song\n");

	while (1) {
		printf("Input beep options:");
		scanf("%d", &cmd);
		while(getchar() != '\n');

		if (cmd == 0) {
			cmd = EXIT_BEEP;
			Ioctl(fd, cmd, &arg);
			break;
		}
			
		switch (cmd) {
			case 1:
				set_beep_voice(fd);
				break;
			case 2:
				cmd = PLAY_SONG;
				Ioctl(fd, cmd, &arg);
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
	if ((fd = open(file, O_RDWR)) < 0) {
		perror("open");
		return -1;
	}
	ret = read(fd, buf, 10);
	if (ret < 0) {
		perror("read");
		return -1;
	}
	close(fd);
	return atoi(buf);
}


void enter_adc(char *s)
{
	int cmd, data, i=10;
	unsigned char arg;
	char *file;

	file = "/sys/bus/iio/devices/iio:device3/in_voltage6_raw";

	printf("<<<< %s >>>>>\n", s);
	printf("=====>>input 0 to quit\n");
	printf("=====>>input 1 to get sensor value\n");

	while (1) {
		i = 10;
		printf("Input options:");
		scanf("%d", &cmd);
		while(getchar() != '\n');

		if (cmd == 0) 
			break;
			
		switch (cmd) {
			case 1:
				while (i--) {
					data = read_data(file);
					if(data < 0) {
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

void enter_light_switch_13pins(int fd)
{
	int cmd, i=10;
	unsigned char arg;


	printf("<<<< light switch >>>>>\n");
	printf("=====>>input 0 to quit\n");
	printf("=====>>input 1 to get status\n");

	while (1) {
		i = 10;
		printf("Input light switch options:");
		scanf("%d", &cmd);
		while(getchar() != '\n');

		if (cmd == 0) 
			break;
		
		switch (cmd) {
			case 1:
				while (i--) {
					cmd = D2_READ;	
					Ioctl(fd, cmd, &arg);
					if (arg == 0)
						printf("there is nothing\n");
					if (arg == 1)
						printf("Oh,there is something!\n");
					sleep(1);
				}
				break;
			default:
				printf("err usage\n");
		}
	}

}

void enter_delay(int fd)
{
	int cmd;
	unsigned char arg;


	printf("<<<< relay >>>>>\n");
	printf("=====>>input 0 to quit\n");
	printf("=====>>input 1 to open relay\n");
	printf("=====>>input 2 to shutdown relay\n");

	while (1) {
		printf("Input touch options:");
		scanf("%d", &cmd);
		while(getchar() != '\n');

		if (cmd == 0) 
			break;
		
		switch (cmd) {
			case 1:
				cmd = D1_WRITE;	
				arg = 1;
				Ioctl(fd, cmd, &arg);
				break;
			case 2:
				cmd = D1_WRITE;
				arg = 0;
				Ioctl(fd, cmd, &arg);
				break;
			default:
				printf("err usage\n");
		}
	}

}

void enter_touch(int fd)
{
	int cmd, i=10;
	unsigned char arg;


	printf("<<<< touch >>>>>\n");
	printf("=====>>input 0 to quit\n");
	printf("=====>>input 1 to get status\n");

	while (1) {
		i = 10;
		printf("Input touch options:");
		scanf("%d", &cmd);
		while(getchar() != '\n');

		if (cmd == 0) 
			break;
		
		switch (cmd) {
			case 1:
				while (i--) {
					cmd = D1_READ;	
					Ioctl(fd, cmd, &arg);
					if (arg == 0)
						printf("nobody touch me\n");
					if (arg == 1)
						printf("I am touched!\n");
					sleep(1);
				}
					break;
			default:
				printf("err usage\n");
		}
	}
}

void enter_infra(int fd)
{
	int cmd, i=10;
	unsigned char arg;


	printf("<<<< infra >>>>>\n");
	printf("=====>>input 0 to quit\n");
	printf("=====>>input 1 to get status\n");

	while (1) {
		i = 10;
		printf("Input infra options:");
		scanf("%d", &cmd);
		while(getchar() != '\n');

		if (cmd == 0) 
			break;
		
		switch (cmd) {
			case 1:
				while (1) {
					cmd = D1_READ;	
					Ioctl(fd, cmd, &arg);
					if (arg == 0)
						printf("0\n");
					if (arg == 1)
						printf("1\n");
					sleep(1);
				}
				break;
			default:
				printf("err usage\n");
		}
	}
}

void read_dht11_data(struct dht11_data *arg)
{
	printf("This is dht11 data : \n");
}

void enter_dht11(int fd)
{
	int cmd, i;
	struct dht11_data arg;

	printf("<<<< dht11 >>>>>\n");
	printf("=====>>input 0 to quit\n");
	printf("=====>>input 1 to get value\n");

	while (1) {
		i = 5;
		printf("Input dht11 options:");
		scanf("%d", &cmd);
		while(getchar() != '\n');

		if (cmd == 0) 
			break;
		
		switch (cmd) {
			case 1:
				while (i--) {
					cmd = READ_DHT11;	
					Ioctl(fd, cmd, &arg);
					read_dht11_data(&arg);
					/*
					 *printf("hu_h : %hhd\n", arg.hu_h);
					 *printf("hu_l : %hhd\n", arg.hu_l);
					 *printf("te_h : %hhd\n", arg.te_h);
					 *printf("te_h : %hhd\n", arg.te_l);
					 */
					printf("hu : %%%02hhd.%02hhd, te: %02hhd.%02hhdC\n", arg.hu_h, arg.hu_l, arg.te_h, arg.te_l);
					sleep(2);
				}
				break;
			default:
				printf("err usage\n");
		}
	}

}

void enter_HCSR04(int fd)
{
	int cmd, i = 10;
	int arg;
	double distance;

	printf("<<<< HC-SR04 >>>>>\n");
	printf("=====>>input 0 to quit\n");
	printf("=====>>input 1 to get value\n");

	while (1) {
		i = 10;
		printf("Input options:");
		scanf("%d", &cmd);
		while(getchar() != '\n');

		if (cmd == 0) 
			break;
		
		switch (cmd) {
			case 1:
				while (i--) {
					cmd = DISTANCE_READ;	
					Ioctl(fd, cmd, &arg);
					distance = 4.8*arg/100;
					printf("distance : %5.2lfcm\n", distance);
					sleep(1);
				}
				break;
			default:
				printf("err usage\n");
		}
	}
}


void get_help()
{
	printf("-----<<CMD LIST>>-----\n");
	printf("666 : quit\n");
	printf("1 : enter fire detect\n");
	printf("2 : enter fan\n"); 
	printf("3 : enter beep\n"); 
	printf("4 : enter RP sensor\n");
	printf("5 : enter light sensor\n");
	printf("6 : enter fire sensor\n");
	printf("7 : enter CO2 sensor\n");
	printf("8 : enter light switch 13pins\n");
	printf("9 : enter delay\n");
	printf("10 : enter touch\n");
	printf("11 : enter infra\n");
	printf("12 : enter dht11\n");
	printf("13 : enter HCSR04\n");
}

int main(int argc, char *argv[])
{
	int fd, ret, num = 0;
	unsigned char arg;
	unsigned long cmd;

	fd = open("/dev/sensor-manage", O_RDWR);
	if(fd < 0) {
		perror("open");
		return -1;
	}

	while (1) {
		printf("\n<<========== %d ==========>>\n", num++);
		printf("Please Input CMD(input 0 to get help) : ");
		scanf("%ld", &cmd);
		while(getchar() != '\n');
		switch (cmd) {
			case 0:
				get_help();
				break;
			case 1:
				enter_fire_detect(fd);
				break;
			case 2:
				enter_fan(fd);
				break;
			case 3:
				enter_beep(fd);
				break;

			case 4:
				enter_adc("RP sensor");
				break;
			case 5:
				enter_adc("light sensor");
				break;
			case 6:
				enter_adc("fire sensor");
				break;
			case 7:
				enter_adc("CO2 sensor");
				break;
			case 8:
				enter_light_switch_13pins(fd);
				break;
			case 9:
				enter_delay(fd);
				break;
			case 10:
				enter_touch(fd);
				break;
			case 11:
				enter_infra(fd);
				break;
			case 12:
				enter_dht11(fd);
				break;
			case 13:
				enter_HCSR04(fd);

			case 666:
				return 0;
				break;


			default:
				printf("<<=====invalid usage=====>>\r\n");
		}
		cmd = 0;
	}
	
	close(fd);

	return 0;
}

