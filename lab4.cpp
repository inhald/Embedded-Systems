#include "LCD_DISCO_F429ZI.h"
#include "TS_DISCO_F429ZI.h"
#include "mbed.h"
#include <cstdio>
#include <time.h>
#include <string>

LCD_DISCO_F429ZI LCD;
TS_DISCO_F429ZI TS;
AnalogIn aIn(PA_0);
PwmOut fan(PD_14);


char buffer2 [30];
bool isOn = false;
float threshold;
bool initialize = false;

void Increase();
void Decrease();

// main() runs in its own thread in the OS
int main()
{
    initialize = false;
    TS_StateTypeDef tsState;
    uint16_t tsX, tsY;
    aIn.set_reference_voltage(3);
    fan.period_us(256);
    uint16_t dInput;

    float max = -10000000000;
    float min = 10000000000; 

    while (true) {
        float temp  = aIn.read_voltage();
        dInput = aIn.read_u16();

        if(temp > max){
            max = temp;
        }
        if(temp < min){
            min = temp;
        }

        float normalized = (temp-min)/(max-min);
        
        if(initialize == false){
            threshold = (temp*100/3)-1 +1;
            sprintf(buffer2, "Thresh: %f", threshold);
            initialize = true;
        }

        // printf("digital 0x%04X\n", dInput);
        //printf("normalized %.6f\n", (temp*100/3)-1);
        //LCD.Clear(LCD_COLOR_WHITE);

        if(temp*100/3-1 > threshold){
            if(!isOn){
                for(int i = 0; i < 256; i++){
                    fan.pulsewidth_us(i);
                    ThisThread::sleep_for(10ms);
                }
                isOn = true;
            }
            else{
                fan.pulsewidth_us(256);
            }
        }
        else{
            // for(int i=256; i > 0; --i){
            //     fan.pulsewidth_us(i);
            //     ThisThread::sleep_for(10ms);
            // }
            fan.pulsewidth_us(0);
            isOn = false;
        }

        thread_sleep_for(500);
        LCD.SetFont(&Font24);
        LCD.Clear(LCD_COLOR_WHITE);
        LCD.SetTextColor(LCD_COLOR_BLACK);
        char buffer1[30];
        sprintf(buffer1, "Sensor: %f", (temp*100/3)-1);
        LCD.DisplayStringAt(0, 20, (uint8_t *)&buffer1, LEFT_MODE);
        LCD.DisplayStringAt(0, 60, (uint8_t *)&buffer2, LEFT_MODE);
        // LCD.SetFont(&Font24);
        // LCD.DisplayStringAt(0,80, (uint8_t *)"+", LEFT_MODE);
        // LCD.DisplayStringAt(50,80, (uint8_t *)"-", LEFT_MODE);
        LCD.SetTextColor(LCD_COLOR_BLUE);
        LCD.FillRect(10,245, 90, 10);
        LCD.FillRect(50,210, 10, 90);
        //LCD.FillRect(0,200, 100, 100);
        //LCD.SetTextColor(LCD_COLOR_);
        //LCD.FillRect(140, 200, 100, 100);
        LCD.FillRect(140,245, 90, 10);

        TS.GetState(&tsState);
        if(tsState.TouchDetected){
            tsX = tsState.X;
            tsY = 320 - tsState.Y;
            if (tsX > 0 && tsY > 200 && tsX < 0+100 && tsY < 200+100) {
                Increase();
                thread_sleep_for(200);
            }else if(tsX > 140 && tsY > 200 && tsX < 140+100 && tsY < 200+100){
                Decrease();           
                thread_sleep_for(200); 
            }

        }

    }

}

void Increase(){
    threshold = threshold + 0.5;
    sprintf(buffer2, "Thresh: %f", threshold);
    LCD.SetTextColor(LCD_COLOR_ORANGE);
    LCD.FillRect(0,200, 100, 100);
            printf("threshold %.6f\n", threshold);
}

void Decrease(){
    threshold = threshold - 0.5;
    sprintf(buffer2, "Thresh: %f", threshold);
    LCD.SetTextColor(LCD_COLOR_ORANGE);
    LCD.FillRect(140, 200, 100, 100);
            printf("threshold %.6f\n", threshold);
}