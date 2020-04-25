cmake_minimum_required(VERSION 3.16)
project(XyOS C)

set(CMAKE_C_STANDARD 11)

include_directories(Bootloader/Headers)
include_directories(Kernel/Headers)
include_directories(SerialClient)

add_executable(XyOS
        Bootloader/Headers/Exceptions.h
        Bootloader/Headers/GPIO.h
        Bootloader/Headers/KernelLoader.h
        Bootloader/Headers/Mailbox.h
        Bootloader/Headers/Peripherals.h
        Bootloader/Headers/UART.h
        Bootloader/Headers/VirtualMemory.h
        Bootloader/Source/ConsoleIO/ConsoleIO.c
        Bootloader/Source/Core/KernelLoader.c
        Bootloader/Source/Core/Main.c
        Bootloader/Source/Exceptions/Exceptions.c
        Bootloader/Source/Peripherals/GPIO/GPIO.c
        Bootloader/Source/Peripherals/GPIO/UART.c
        Bootloader/Source/Peripherals/GPU/Mailbox.c
        Bootloader/Source/VirtualMemory/VirtualMemory.c
        Kernel/Headers/ARMTimer.h
        Kernel/Headers/ConsoleFont.h
        Kernel/Headers/ConsoleIO.h
        Kernel/Headers/Exceptions.h
        Kernel/Headers/FrameBuffer.h
        Kernel/Headers/FrameBufferGraphics.h
        Kernel/Headers/GPIO.h
        Kernel/Headers/InterruptController.h
        Kernel/Headers/Mailbox.h
        Kernel/Headers/Menu.h
        Kernel/Headers/Peripherals.h
        Kernel/Headers/SystemTimer.h
        Kernel/Headers/Threading.h
        Kernel/Headers/TrafficLights.h
        Kernel/Headers/UART.h
        Kernel/Headers/VirtualMemory.h
        Kernel/Source/ConsoleIO/ConsoleIO.c
        Kernel/Source/Core/Main.c
        Kernel/Source/Core/Menu.c
        Kernel/Source/Exceptions/Exceptions.c
        Kernel/Source/Peripherals/ARMTimer/ARMTimer.c
        Kernel/Source/Peripherals/GPIO/GPIO.c
        Kernel/Source/Peripherals/GPIO/TrafficLights.c
        Kernel/Source/Peripherals/GPIO/UART.c
        Kernel/Source/Peripherals/GPU/FrameBuffer.c
        Kernel/Source/Peripherals/GPU/FrameBufferGraphics.c
        Kernel/Source/Peripherals/GPU/Mailbox.c
        Kernel/Source/Peripherals/InterruptController/InterruptController.c
        Kernel/Source/Peripherals/SystemTimer/SystemTimer.c
        Kernel/Source/Scheduler/Threading.c
        Kernel/Source/VirtualMemory/VirtualMemory.c
        SerialClient/GPIO.c
        SerialClient/GPIO.h
        SerialClient/Peripherals.h
        SerialClient/SerialClient.c
        SerialClient/UART.c
        SerialClient/UART.h)
