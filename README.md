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

## Build with Docker (recommended)

> The Docker files live in the `docker/` folder.

### 1) Build the Docker image
```bash
docker build -t stm32-alpine:latest docker

### 2) Run a container with the project mounted

docker run --rm -it \
  --name stm32-build \
  -v "$PWD":/work \
  -w /work \
  stm32-alpine:latest \
  bash

    Windows PowerShell: use -v "${PWD}:/work".
    CMD: use -v %cd%:/work.

### 3) Configure & build inside the container

cmake -S . -B build -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain-arm-none-eabi.cmake \
  -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel

Artifacts (e.g. *.elf, *.bin) will be in build/.


Flash using ST-Link:

st-flash write build/<firmware>.bin 0x08000000

License

MIT