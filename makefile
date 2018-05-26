GCC		= arm-none-eabi-gcc
OBJCOPY		= arm-none-eabi-objcopy
GCC_FLAGS	= -nostartfiles -O2 -mfpu=neon-vfpv4 -mfloat-abi=hard -march=armv7-a -mtune=cortex-a7 -o $(OUTPUT).elf 
OBJCOPY_FLAGS	= $(OUTPUT).elf -O binary $(OUTPUT).img
CODE		= $(wildcard *.c)
HEADERS		= $(wildcard *.h)
OUTPUT		= kernel7
SD_CARD		= /media/xbhatnag/boot/

build: $(CODE) $(HEADER)
	$(GCC) $(GCC_FLAGS) $(CODE)
	$(OBJCOPY) $(OBJCOPY_FLAGS)
copy: $(OUTPUT).img
	cp $(OUTPUT).img $(SD_CARD)
umount:
	umount /dev/sda1
	umount /dev/sda2
clean:
	rm $(OUTPUT).*
