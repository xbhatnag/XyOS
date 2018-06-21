TOOLCHAIN	= aarch64-none-elf
GCC		= $(TOOLCHAIN)-gcc
OBJCOPY		= $(TOOLCHAIN)-objcopy
GDB		= $(TOOLCHAIN)-gdb
QEMU		= qemu-system-aarch64
C_FLAGS		= -Wall -O2 -ffreestanding -nostartfiles
C_FILES		= $(wildcard *.c)
C_OBJS		= $(C_FILES:.c=.o)
ASM_FLAGS	= 
ASM_FILES	= $(wildcard *.S)
ASM_OBJS	= $(ASM_FILES:.S=.o)
LINK_FLAGS	= -nostdlib -nostartfiles
OUTPUT		= kernel8
SD_CARD		= /Volumes/boot
LINKER		= linker.ld
GDB_CONF	= gdb.conf

$(OUTPUT).img: $(ASM_OBJS) $(C_OBJS)
	$(GCC) $(LINK_FLAGS) $(C_OBJS) $(ASM_OBJS) -T $(LINKER) -o $(OUTPUT).elf
	$(OBJCOPY) $(OUTPUT).elf -O binary $(OUTPUT).img

%.o: %.c
	$(GCC) $(C_FLAGS) -c $< -o $@

%.o: %.S
	$(GCC) $(ASM_FLAGS) -c $< -o $@

copy: $(OUTPUT).img
	rm $(SD_CARD)/$(OUTPUT).img
	cp $(OUTPUT).img $(SD_CARD)

clean:
	rm $(C_OBJS) $(ASM_OBJS) $(OUTPUT).*

emulate:
	$(QEMU) -M raspi3 -kernel $(OUTPUT).img -s -S -serial stdio

gdb: $(GDB_CONF) $(OUTPUT).img
	$(GDB) --command=$(GDB_CONF)
