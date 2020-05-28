/* 
	Editor: https://www.visualmicro.com/
			visual micro and the arduino ide ignore this code during compilation. this code is automatically maintained by visualmicro, manual changes to this file will be overwritten
			the contents of the _vm sub folder can be deleted prior to publishing a project
			all non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
			note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: Generic STM32F103C series, Platform=STM32F1, Package=stm32duino
*/

#define _VMDEBUG 1
#define ARDUINO 10809
#define ARDUINO_MAIN
#define F_CPU 72000000L
#define printf iprintf
#define __STM32F1__
#define DEBUG_LEVEL DEBUG_NONE
#define BOARD_generic_stm32f103c
#define VECT_TAB_ADDR 0x8000000
#define ERROR_LED_PORT GPIOC
#define ERROR_LED_PIN 13
#define F_CPU 72000000L
#define ARDUINO 10809
#define ARDUINO_GENERIC_STM32F103C
#define ARDUINO_ARCH_STM32F1
#define CONFIG_MAPLE_MINI_NO_DISABLE_DEBUG
#define MCU_STM32F103C8
#define __STM32F1__
#include "pins_arduino.h" 
#include "variant.h" 
#include "arduino.h"
#include "SmartCafe.ino"
