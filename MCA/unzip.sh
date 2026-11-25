#!/bin/bash

setup_stm32() {
    #sudo apt-get update -qq || return 1
    #sudo apt-get install -y -qq p7zip-full >/dev/null || return 1

    7z x -y stm32f103c8t6.7z >/dev/null || return 1
    7z x -y rtos.7z >/dev/null || return 1
    7z x -y FreeRTOS.7z >/dev/null || return 1

    cp -a rtos/. stm32f103c8t6/rtos/ || return 1
    cp -a FreeRTOS stm32f103c8t6/ || return 1

    rm -rf rtos FreeRTOS

    cd stm32f103c8t6
}

setup_stm32
