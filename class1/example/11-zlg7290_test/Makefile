CC:= /opt/st/stm32mp1/3.1-openstlinux-5.4-dunfell-mp1-20-06-24/sysroots/x86_64-ostl_sdk-linux/usr/bin/arm-ostl-linux-gnueabi/arm-ostl-linux-gnueabi-gcc
CFLAGS:= -mthumb -mfpu=neon-vfpv4 -mfloat-abi=hard -mcpu=cortex-a7 \
	--sysroot=/opt/st/stm32mp1/3.1-openstlinux-5.4-dunfell-mp1-20-06-24/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi
LIBS:= 
KEY_OUT:=keyboard_test
LED_OUT:=led_test

.PHONY: clean

all:$(KEY_OUT) $(LED_OUT)

$(KEY_OUT): keyboard_test.o
	$(CC)  $(CFLAGS) $^ -o $@

$(LED_OUT): led_test.o
	$(CC)  $(CFLAGS) $^ -o $@

%*.o: %*.c
	$(CC) $(CFLAGS) $^ -o $@ 

clean:
		rm $(KEY_OUT) $(LED_OUT) *.o

