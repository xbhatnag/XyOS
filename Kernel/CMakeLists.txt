cmake_minimum_required(VERSION 3.16)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR aarch64)
set(CMAKE_CROSSCOMPILING 1)

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

project(Kernel C ASM)
set(CMAKE_C_STANDARD 11)

set(tools /usr/local/opt/aarch64-none-elf)
set(CMAKE_FORCE_C_COMPILER ${tools}/bin/aarch64-none-elf-gcc)
set(CMAKE_FORCE_CXX_COMPILER ${tools}/bin/aarch64-none-elf-gcc)
set(OBJCOPY ${tools}/bin/aarch64-none-elf-objcopy)

set(CMAKE_C_FLAGS "-Wall -g -O3 -ffreestanding -nostartfiles -IHeaders -mcpu=cortex-a53+nofp")
set(CMAKE_C_LINK_FLAGS "")
set(CMAKE_EXE_LINKER_FLAGS "-nostdlib -nostartfiles -T ${CMAKE_SOURCE_DIR}/Linker.ld")

include_directories(Headers)
FILE(GLOB_RECURSE CSources ./Source/*.c)
FILE(GLOB_RECURSE ASMSources ./Source/*.S)
FILE(GLOB_RECURSE Headers ./Headers/*.h)

add_executable(Kernel
        ${Headers}
        ${CSources}
        ${ASMSources})

add_custom_command(TARGET Kernel 
        POST_BUILD
        COMMAND ${OBJCOPY} Kernel -O binary Kernel.img
        COMMENT "Creating binary...")