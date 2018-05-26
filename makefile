build: main.c
	arm-none-eabi-gcc -nostartfiles -O2 -mfpu=neon-vfpv4 -mfloat-abi=hard -march=armv7-a -mtune=cortex-a7 -o kernel7.elf main.c
	arm-none-eabi-objcopy kernel7.elf -O binary kernel7.img
copy: kernel7.img
	cp kernel7.img /media/xbhatnag/boot/
umount:
	umount /dev/sda1
	umount /dev/sda2
clean:
	rm kernel7.img kernel7.elf
