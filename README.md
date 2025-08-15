# STM32H743 Hello World (UART)

## Overview
This project is a minimal firmware for the STM32H743 microcontroller that periodically outputs  
`Hello World` to a UART console. It uses STM32Cube HAL drivers and FreeRTOS for task scheduling.

## Features
- **MCU:** STM32H743
- **UART:** Configurable baud rate (default: 115200 bps, 8-N-1)
- **RTOS:** FreeRTOS task to send `"Hello World"` every 1 second
- **Build system:** CMake + Ninja
- **Toolchain:** arm-none-eabi-gcc

## Hardware Setup
- Connect UART TX pin of STM32H743 to USB-UART adapter (default: USART3 / PD8)
- Configure your serial monitor to `115200 bps`

## Build & Flash
```bash
cmake -S . -B build -G Ninja
cmake --build build --parallel

Flash using ST-Link:

st-flash write build/<firmware>.bin 0x08000000

License

MIT