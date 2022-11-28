CC:= /opt/st/stm32mp1/3.1-openstlinux-5.4-dunfell-mp1-20-06-24/sysroots/x86_64-ostl_sdk-linux/usr/bin/arm-ostl-linux-gnueabi/arm-ostl-linux-gnueabi-gcc
CFLAGS:= -mthumb -mfpu=neon-vfpv4 -mfloat-abi=hard -mcpu=cortex-a7 \
	--sysroot=/opt/st/stm32mp1/3.1-openstlinux-5.4-dunfell-mp1-20-06-24/sysroots/cortexa7t2hf-neon-vfpv4-ostl-linux-gnueabi
OBJS:= si7006_test.o 
LIBS:= 
OUT:=si7006_test

.PHONY: clean

$(OUT): $(OBJS)
		$(CC)  $(CFLAGS) $^ -o $@

%*.o: %*.c
		$(CC) $(CFLAGS) $^ -o $@ 

clean:
		rm $(OUT) $(OBJS)

