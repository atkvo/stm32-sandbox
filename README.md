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
cmake ..
make
```

# Flashing programs

```sh
# assume currnt working directory is in the build output folder
# and the binary is called test.bin
# the flash address starts at 0x08000000
st-flash write test.bin 0x08000000
```