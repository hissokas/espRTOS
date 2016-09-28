#!/bin/bash

export SDK_PATH=~/Repos/espRTOS/server/ESP8266_RTOS_SDK
export BIN_PATH=~/Repos/espRTOS/server/ESP8266_RTOS_SDK/bin

make clean

make BOOT=none APP=0 SPI_SPEED=40 SPI_MODE=QIO SPI_SIZE_MAP=6
