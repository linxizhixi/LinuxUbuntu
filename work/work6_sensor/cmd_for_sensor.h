#ifndef __CMD_FSMP1C_SENSOR
#define __CMD_FSMP1C_SENSOR


#define SENSOR_MAGIC 'f'
#define SENSOR_MAX_NR 100

/*
 *#define INIT_FIRE_DETECT (_IO('f', 1))
 *#define READ_FIRE_DETECT (_IOR('f', 2, unsigned char))
 *#define EXIT_FIRE_DETECT (_IO('f', 3))
 *
 */
#define INIT_FAN (_IO('f', 4))
#define FAN_UP (_IO('f', 5))
#define FAN_DOWN (_IO('f', 6))
#define EXIT_FAN (_IO('f', 7))

#define INIT_BEEP (_IO('f', 8))
#define BEEP_VOICE_UP (_IO('f', 9))
#define BEEP_VOICE_DOWN (_IO('f', 10))
#define EXIT_BEEP (_IO('f', 11))

#define PLAY_SONG (_IO('f', 100))

#define D2_READ (_IOR('f', 12, unsigned char))
#define D2_WRITE (_IOW('f', 13, unsigned char))
//#define D2_EXIT ( _IO('f', 14))
#define D1_READ (_IOR('f', 15, unsigned char))
#define D1_WRITE (_IOW('f', 16, unsigned char))
//#define D1_EXIT ( _IO('f', 17))
//#define D1_INIT ( _IO('f', 18))
//#define D2_INIT ( _IO('f', 19))

#define F5_READ ( _IOR('f', 20, unsigned char))

#define READ_DHT11 (_IOR('f', 21, struct dht11_data))

#define DISTANCE_READ (_IOR('f', 22, int))


struct dht11_data {
	unsigned char hu_h;
	unsigned char hu_l;
	unsigned char te_h;
	unsigned char te_l;
	unsigned char check;
};


#endif 
