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

int main()  
{  
	int s, nbytes, i;
	char *device="can0";
	int id = 0x22; 
	struct sockaddr_can addr;  
	struct ifreq ifr;  
	struct can_frame frame;  
	struct can_filter rfilter[1];

	/* create a socket */  
	s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
	strcpy(ifr.ifr_name, device);
	/* determine the interface index */  
	ioctl(s, SIOCGIFINDEX, &ifr);                    
	addr.can_family = AF_CAN;  
	addr.can_ifindex = ifr.ifr_ifindex;
	/* bind the socket to a CAN interface */    
	bind(s, (struct sockaddr *)&addr, sizeof(addr));

	/* define the filter rules */   
	rfilter[0].can_id   = id;  
	rfilter[0].can_mask = CAN_SFF_MASK;  
	/* Set the filter rules */	  
	setsockopt(s, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter)); 

	while(1) {
		/* receive frame */  
		nbytes = read(s, &frame, sizeof(frame));            
		/* printf the received frame */  
		if (nbytes > 0) { 
			printf("%s  %#x  [%d]  ", ifr.ifr_name, frame.can_id, frame.can_dlc);
			for (i = 0; i < frame.can_dlc; i++)
				printf("%c", frame.data[i]); 
			printf("\n"); 
		}  
	}  
	close(s);  
	return 0;  
}  
