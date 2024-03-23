# Embedded Systems Design I 
Written in C++ using Keil IDE
## BlinkerISR: A Simple FSM using Interrupts
Implementation of an FSM cycling through different blinking states for leds. 
## Reaction-Time Tester
Design of reaction-time tester, which calculates and displays results on an LCD. The tester records both the most recent and best times, also has a debounced external switch permitting for the resetting of the LCD screen and times. 

To use this code, [BSP](https://os.mbed.com/teams/Embedded-System-Design-with-ARM-Cortex-M/code/BSP_DISCO_F429ZI/) and [LCD](https://os.mbed.com/teams/Embedded-System-Design-with-ARM-Cortex-M/code/LCD_DISCO_F429ZI/) libraries must be added to Keil Studio. 

[Demo](https://www.youtube.com/watch?v=tGkoB--rwNQ)


## Real-Time Clock
Simple real time clock on STM32F4 Discovery Board implemented in C++ using Keil IDE.

Features:
1. Detects last two times at which user button was pressed and stores them on 24FC64F EEPROM integrated circuit by reading and writing data using I2C. 
2. Displays time (HH:MM:SS) on LCD
3. Additional external push buttons are used to set time and date

To use this code, [BSP](https://os.mbed.com/teams/Embedded-System-Design-with-ARM-Cortex-M/code/BSP_DISCO_F429ZI/), and [LCD](https://os.mbed.com/teams/Embedded-System-Design-with-ARM-Cortex-M/code/LCD_DISCO_F429ZI/).

[Demo](https://www.youtube.com/watch?v=vtHhaU6ryDQ)



  
