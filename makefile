# Program Variables
TOOLCHAIN	= aarch64-none-elf
GCC		= $(TOOLCHAIN)-gcc
OBJCOPY		= $(TOOLCHAIN)-objcopy

# Libraries
LIBRARIES_C_FLAGS		= -Wall -O2 -ffreestanding -nostartfiles -ILibraries/Headers
LIBRARIES_C_FILES		= $(shell find Libraries -name "*.c")
LIBRARIES_C_OBJECTS	= $(C_FILES:.c=.o)

# C Variables
C_FLAGS		= -Wall -O2 -ffreestanding -nostartfiles -IBootloader/Headers -ILibraries/Headers
C_FILES		= $(shell find Bootloader -name "*.c")
C_OBJECTS	= $(C_FILES:.c=.o)

# Assembly Variables
ASM_FLAGS	=
ASM_FILES	= $(shell find Bootloader -name "*.S")
ASM_OBJECTS	= $(ASM_FILES:.S=.o)

# Linker Variables
LINK_FLAGS	= -nostdlib -nostartfiles
LINKER_FILE	= Linker.ld

# Output Variables
OUTPUT		= Bootloader

$(OUTPUT).img: $(ASM_OBJECTS) $(C_OBJECTS) $(LIBRARIES_C_OBJECTS) $(LINKER_FILE)
	$(GCC) $(LINK_FLAGS) $(C_OBJECTS) $(ASM_OBJECTS) $(LIBRARIES_C_OBJECTS) -T $(LINKER_FILE) -o $(OUTPUT).elf
	$(OBJCOPY) $(OUTPUT).elf -O binary $(OUTPUT).img

%.o: %.c
	$(GCC) $(C_FLAGS) -c $< -o $@

%.o: %.S
	$(GCC) $(ASM_FLAGS) -c $< -o $@

clean:
	rm $(C_OBJECTS) $(ASM_OBJECTS) $(OUTPUT).*
