#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <net/if.h> 
#include <sys/ioctl.h> 
#include <sys/socket.h> 
#include <linux/can.h> 
#include <linux/can/raw.h>
#include <getopt.h>

int main(int argc, char *argv[])  
{  
    int s, nbytes, i; 
    char *device = "can0";
    int id = 0x22 ;   
	struct sockaddr_can addr;  
	struct ifreq ifr;  
	struct can_frame frame[1];

	if (argc < 2) {
		printf("Usage: ./cansend [data]\n");
		return 1;
	}
	
	/* create a socket */  
	s = socket(PF_CAN, SOCK_RAW, CAN_RAW);      
	if (s < 0) {
		perror("socket");
	}
	strcpy(ifr.ifr_name, device );

	/* determine the interface index */  
	ioctl(s, SIOCGIFINDEX, &ifr);                     
	addr.can_family = AF_CAN;  
	addr.can_ifindex = ifr.ifr_ifindex;

	/* bind the socket to a CAN interface */  
	bind(s, (struct sockaddr *)&addr, sizeof(addr));              

	/* Set the filter rules */  
	setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);  

	/* generate CAN frames */ 
	frame[0].can_id = id;  
	frame[0].can_dlc = strlen(argv[1]);
	for(i = 0; i < frame[0].can_dlc; i++) {
		frame[0].data[i] = argv[1][i];	
	}
	 
	/* send CAN frames */  
	while(1) {  
		nbytes = write(s, &frame[0], sizeof(frame[0]));    
		if (nbytes < 0) {
            perror("can raw socket write");
		        return 1;
		}

		/* paranoid check ... */
		if (nbytes < sizeof(struct can_frame)) {
		        fprintf(stderr, "read: incomplete CAN frame\n");
		        return 1;
		} 
		usleep(1000000);
	}  
	close(s);  
	return 0;  
}  
