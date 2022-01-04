# STM32 SANDBOX

This project is just intended to become more familiar with the
bare metal STM32 environment including:

* Build system (Cmake)
* HAL interface development
* Device drivers
* Unit testing embedded firmware.
* Debugging via OpenOCD/GDB/ST-Link

There is no specific project/application in mind at the moment. Just an ongoing learning exercise.

The initial STM32 development board is the [WeAct Black Pill V2.0](https://stm32-base.org/boards/STM32F411CEU6-WeAct-Black-Pill-V2.0.html)

# Requirements

* `arm-none-eabi-gcc` tools
* `st-link` tools
* `cmake`

## Mac OS

Assuming `homebrew` is installed

```
brew install gcc-arm-embedded
brew install cmake
brew install stlink
brew install openocd
```

## Linux 

On Ubuntu install `cmake` and `gcc-arm-none-eabi` tools

```sh
sudo apt install gcc-arm-none-eabi
sudo apt install cmake
```

To install the latest `st-link` tools, download the `*.deb` package from https://github.com/stlink-org/stlink and install via `apt`

```sh
sudo apt install path/to/the/st/link/deb/file
```

# Build

```sh
# create a build folder for an out of source build
mkdir build 
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../arm-none-eabi-gcc.cmake   
make
```

# Flashing programs

> note: if using st-flash, flash using the binary file (`*.bin`) at the flash address (stm32f4: `0x08000000`)

```sh
# assume current working directory is in the build output folder
# and the binary is called test.bin
# the flash address starts at 0x08000000
st-flash write test.bin 0x08000000

```

## OpenOCD

Flashing instructions here: https://openocd.org/doc/html/Flash-Programming.html

> For `*.bin` files - you must specify the address to flash the program to (stm32f4: `0x08000000`)
> For `*.elf` files - you do *not* need to specify the address where to flash the program

## Via OpenOCD Telnet Interface

```sh
# start the open ocd instance from the shell with the proper debugger/board configuration
openocd -f /usr/local/Cellar/open-ocd/0.11.0/share/openocd/scripts/interface/stlink-v2-1.cfg -f /usr/local/Cellar/open-ocd/0.11.0/share/openocd/scripts/target/stm32f4x.cfg

# in a separate shell instance, telnet into the openocd server
telnet localhost:4444

# from the telnet session 
targets     # use this to see run state of MCU
reset halt  # reset and halt the MCU
program test.elf verify reset
```

#### One Liner

```sh
openocd -f /usr/local/Cellar/open-ocd/0.11.0/share/openocd/scripts/interface/stlink-v2-1.cfg -f /usr/local/Cellar/open-ocd/0.11.0/share/openocd/scripts/target/stm32f4x.cfg -c "program stm32-test.elf verify reset"
```