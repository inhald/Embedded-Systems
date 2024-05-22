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

To use this code, [BSP](https://os.mbed.com/teams/Embedded-System-Design-with-ARM-Cortex-M/code/BSP_DISCO_F429ZI/), and [LCD](https://os.mbed.com/teams/Embedded-System-Design-with-ARM-Cortex-M/code/LCD_DISCO_F429ZI/) must be added to Keil Studio.

[Demo](https://www.youtube.com/watch?v=vtHhaU6ryDQ)

## PWM-Fan
A Pulsewidth Modulation controlled fan triggered by temperature falling below a specified threshold by the user.

Features:
1. Detects press on LCD touchscreen to either increment or decrement the threshold.
2. Samples measurements using temperature sensor, which are then amplified electronically and read as an analog value on the microcontroller. 
3. When temperature is above the threshold, fan is activated and cools the temperature sensor.


To use this code, [BSP](https://os.mbed.com/teams/Embedded-System-Design-with-ARM-Cortex-M/code/BSP_DISCO_F429ZI/), [LCD](https://os.mbed.com/teams/Embedded-System-Design-with-ARM-Cortex-M/code/LCD_DISCO_F429ZI/) and [Touchscreen](https://os.mbed.com/teams/ST/code/TS_DISCO_F429ZI/file/4f8b6df8e235/TS_DISCO_F429ZI.h/) must be added to Keil Studio.


## Open-Ended Midterm Project - Self Balancing Robot
Task: Create a cybernetic system of your own design which
senses its environment and produces responsive outputs/actions.

Description: The system is a self-balancing robot that corrects tilts in its angle, returning the robot to a more stable position. It consists of a sturdy chassis, motor control circuit, stm32 microcontroller and an external accelerometer. The chassis is made of plexiglass and has two layers. The lower layer holds all the circuitry, namely, the h-bridge driver and the external power supply. Under this layer, there are two motors that control the wheels of the robot, these are connected to the H-bridge which is controlled by the STM32 microcontroller. The top layer supports the STM32 microcontroller, which is also connected accelerometer and interfaces with it using I2C. The two layers are connected using four studs, which ensures that both surfaces are parallel, and that gyroscope measurements are as accurate as possible. The software loaded on the STM32 microcontroller takes measurements from both the accelerometer and gyroscope (and normalizes them), fuses them using a complementary/linear Kalman filter, then applies a PD control algorithm which sends a PWM signal to the motors via the h-bridge driver circuit. Feedback data from the program is also displayed on an LCD.


[Demo](https://www.youtube.com/watch?v=SOBp2ADBT7k)




  
