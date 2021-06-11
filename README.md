
# AI-deck-workshop

<img style="float: right;" src="imgs/overview.png" width="100%">


## 1. Basic programming

Hanna: please describe here this hands-on session


## 2. Image manipulation

Hanna: please describe here this hands-on session


## 3. Firmware integration

This example shows how to communicate via UART between the AI-deck and the STM32 in the Crazyflie. The example consists of two parts: the code to be flashed in the GAP8 and the Crazyflie application.
1. The GAP8 code initializes the UART and then sends a counter value every 0.5ms.
2. The Crazyflie application shows how to use UART together with DMA.
Whenever the counter value is received from GAP8, an interrupt is triggered in the STM32, which signals the application to print the counter value in the console.

## 4. Video streaming 

Lorenzo: please describe here this hands-on session