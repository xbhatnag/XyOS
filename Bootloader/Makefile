# Program Variables
TOOLCHAIN	= aarch64-none-elf
GCC		= $(TOOLCHAIN)-gcc
OBJCOPY		= $(TOOLCHAIN)-objcopy

# C Variables
C_FLAGS		= -Wall -O2 -ffreestanding -nostartfiles -IHeaders
C_FILES		= $(shell find . -name "*.c")
C_OBJECTS	= $(C_FILES:.c=.o)

# Assembly Variables
ASM_FLAGS	=
ASM_FILES	= $(shell find . -name "*.S")
ASM_OBJECTS	= $(ASM_FILES:.S=.o)

# Linker Variables
LINK_FLAGS	= -nostdlib -nostartfiles
LINKER_FILE	= Linker.ld

# Output Variables
OUTPUT		= Bootloader

$(OUTPUT).img: $(ASM_OBJECTS) $(C_OBJECTS) $(LINKER_FILE)
	$(GCC) $(LINK_FLAGS) $(C_OBJECTS) $(ASM_OBJECTS) -T $(LINKER_FILE) -o $(OUTPUT).elf
	$(OBJCOPY) $(OUTPUT).elf -O binary $(OUTPUT).img

%.o: %.c
	$(GCC) $(C_FLAGS) -c $< -o $@

%.o: %.S
	$(GCC) $(ASM_FLAGS) -c $< -o $@

clean:
	rm $(C_OBJECTS) $(ASM_OBJECTS) $(OUTPUT).*
