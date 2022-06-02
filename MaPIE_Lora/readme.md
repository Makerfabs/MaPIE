# Makerfabs MaPIE Lora Radio



```c++
/*
Version:		V1.0
Author:			Vincent
Create Date:	2022/6/2
Note:
				V1.0 Create Project
			
*/
```

[toc]

# Makerfabs

[Makerfabs home page](https://www.makerfabs.com/)

[Makerfabs Wiki](https://makerfabs.com/wiki/index.php?title=Main_Page)



# What is MaPIE Lora Radio

MaPIE Lora Radio is the first product in the MaPIE series.

There are onboard Lora modules and Micro SD card slots.

## Feature

- Raspberry RP2040 MCU
- Lora frequency: 433M/ 868M/ 915M
- Support Micro SD Card



# How To Use

## Complier Option

**If you have any questions，such as how to install the development board, how to download the code, how to install the library. Please refer to :[Makerfabs_FAQ](https://github.com/Makerfabs/Makerfabs_FAQ)**

- Please disconnect from the electrical equipment before burning!
- Do not burn with alternating current!
- Install library: RadioLib and other libs in Arduino IDE
- Upload codes, select "Arduino Mbed OS RP2040 Boards" and "Raspberry Pi Pico"

MaPIE Lora Radio use this Lora library: [RadioLib](https://github.com/jgromes/RadioLib)

**It is recommended to install from Arduino Library Manager and select the library version 4.6.0. RadioLib libraries after 5.0 need to add "RADIOLIB_" before some macro definitions.**

The current firmware is developed in RadioLib 4.6.0.



## Firmware

### MaPIE_LORA_V1.0 

The main function is to verify SD card and provide simple Lora reception.

Some SD cards tested are incompatible or have a low success rate, such as the Kingston 8G.