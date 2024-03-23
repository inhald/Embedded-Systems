#include "LCD_DISCO_F429ZI.h"
#include "DebouncedInterrupt.h"
#include "fonts.h"
#include "mbed.h"
#include <cstdint>
#include <cstring>
#include <time.h>
#include <string.h>


#define SDA_PIN PC_9
#define SCL_PIN PA_8
#define EEPROM_ADDR 0xA0

LCD_DISCO_F429ZI LCD;

unsigned int eeprom_addr = 0;

I2C i2c(SDA_PIN, SCL_PIN);

void WriteEEPROM(int address, unsigned int ep_address, char *data, int size);
void ReadEEPROM(int address, unsigned int ep_address, char *data, int size);

//internal
InterruptIn userButton(BUTTON1);


//external buttons
DebouncedInterrupt button1(PA_5);
DebouncedInterrupt button2(PA_7);
DebouncedInterrupt button3(PA_6);


int button1counter = 0;


//reading EEPROM
char data_read[16];


//writing to EEPROM
char writebuf[16];
bool eewrite = false;

//times
char time1[8];
char time2[8];


//setting time on LCD
int hours = 0; int minutes =0; int seconds = 0;
int highlightcount = 0;
tm t;
bool timeset = false;


void button2ISR(){
    highlightcount++;
}

void button1setISR(){
    if(highlightcount % 4 ==0){
        hours++;
        if(hours > 12) hours = 0;
    }
    else if(highlightcount % 4 == 1){
        minutes++;
        if(minutes > 60) minutes = 0;
    }
    else if(highlightcount % 4 == 2){
        seconds++;
        if(seconds > 60) seconds = 0;
    }

}

void IntButtonISR(){
    eewrite = true;
}



void state0(){
    
    LCD.Clear(LCD_COLOR_WHITE);
    LCD.SetBackColor(LCD_COLOR_ORANGE); //Sets back to orange
    LCD.SetTextColor(LCD_COLOR_BLACK);

    time_t rawtime;
    time(&rawtime); //sets rawtime to current time
    char timebuf[20];
    strftime(timebuf, 20, "%H:%M:%S", localtime(&rawtime));

    LCD.SetFont(&Font24);
    LCD.DisplayStringAt(0, 80, (uint8_t *)&timebuf, CENTER_MODE);

}

void state1(){

    LCD.Clear(LCD_COLOR_WHITE);
    LCD.SetFont(&Font24);
    LCD.SetBackColor(LCD_COLOR_ORANGE); //Sets back to orange
    LCD.SetTextColor(LCD_COLOR_BLACK);

    ReadEEPROM(EEPROM_ADDR, eeprom_addr, data_read, 16);

    char buffer1[9];
    char buffer2[9];

    memcpy(buffer1, data_read, 8);
    buffer1[8] = '\0';

    memcpy(buffer2, data_read+8, 8);
    buffer2[8] = '\0';


    //write times
    LCD.DisplayStringAt(0,80, (uint8_t *)&buffer1, CENTER_MODE);
    LCD.DisplayStringAt(0,160,(uint8_t *)&buffer2, CENTER_MODE);
    
}


void state2(){

    button1.attach(&button1setISR, IRQ_FALL, 1000, true);
    button2.attach(&button2ISR, IRQ_FALL, 1000, true);

    LCD.Clear(LCD_COLOR_WHITE);
    LCD.SetFont(&Font16);
    LCD.SetTextColor(LCD_COLOR_BLACK);
    LCD.SetBackColor(LCD_COLOR_WHITE);


    char buffer1[16];
    sprintf(buffer1, " Hours: %d", hours);

    char buffer2[16];
    sprintf(buffer2, " Minutes: %d", minutes);

    char buffer3[16];
    sprintf(buffer3, " Seconds: %d", seconds);

    char buffer4[16];
    sprintf(buffer4, "Save and Exit");

    //highlighting 
    
    if(highlightcount % 4 == 0){
        LCD.SetBackColor(LCD_COLOR_ORANGE);
        LCD.DisplayStringAt(0,40, (uint8_t *)&buffer1, LEFT_MODE);
        LCD.SetBackColor(LCD_COLOR_WHITE);
        LCD.DisplayStringAt(0,80, (uint8_t*)&buffer2, LEFT_MODE);
        LCD.DisplayStringAt(0,120, (uint8_t*)&buffer3, LEFT_MODE);
        LCD.DisplayStringAt(0,200, (uint8_t*)&buffer4, CENTER_MODE);
    }
    else if(highlightcount % 4 == 1){
        LCD.SetBackColor(LCD_COLOR_ORANGE);
        LCD.DisplayStringAt(0,80, (uint8_t*)&buffer2, LEFT_MODE);
        LCD.SetBackColor(LCD_COLOR_WHITE);
        LCD.DisplayStringAt(0,40, (uint8_t *)&buffer1, LEFT_MODE);
        LCD.DisplayStringAt(0,120, (uint8_t*)&buffer3, LEFT_MODE);
        LCD.DisplayStringAt(0,200, (uint8_t*)&buffer4, CENTER_MODE);

    }
    else if(highlightcount % 4 == 2){
        LCD.SetBackColor(LCD_COLOR_ORANGE);
        LCD.DisplayStringAt(0,120, (uint8_t*)&buffer3, LEFT_MODE);
        LCD.SetBackColor(LCD_COLOR_WHITE);
        LCD.DisplayStringAt(0,40, (uint8_t *)&buffer1, LEFT_MODE);
        LCD.DisplayStringAt(0,80, (uint8_t*)&buffer2, LEFT_MODE);
        LCD.DisplayStringAt(0,200, (uint8_t*)&buffer4, CENTER_MODE);
        
    }
    else{
        LCD.SetBackColor(LCD_COLOR_ORANGE);
        LCD.DisplayStringAt(0,200, (uint8_t*)&buffer4, CENTER_MODE);
        LCD.SetBackColor(LCD_COLOR_WHITE);
        LCD.DisplayStringAt(0,40, (uint8_t *)&buffer1, LEFT_MODE);
        LCD.DisplayStringAt(0,80, (uint8_t*)&buffer2, LEFT_MODE);
        LCD.DisplayStringAt(0,120, (uint8_t*)&buffer3, LEFT_MODE);

    }
}



typedef enum {STATE0 = 0, STATE1 = 1, STATE2 = 2} State_Type;
State_Type current_state = STATE0;

void (*state_table[])(){state0, state1, state2};


void button1ISR(){
    if(button1counter % 2 == 0){
        current_state = STATE1;
        button1counter++;
    }
    else{
        current_state = STATE0;
        button1counter++;
    }
}


void button3ISR(){
    if(current_state == STATE0){
        current_state = STATE2;
    }
    else if(highlightcount % 4 == 3){
        current_state = STATE0;
        t.tm_hour = hours;
        t.tm_min = minutes;
        t.tm_sec = seconds;
        timeset = true;
    }
}






int main() {

  // Read and write a byte from/to EEPROM

  //rtc
    t.tm_year = 124;
    t.tm_mon = 0;
    t.tm_mday = 0;
    t.tm_hour = 0;
    t.tm_min = 0;
    t.tm_sec = 0;
    set_time(mktime(&t));


    LCD.Clear(LCD_COLOR_WHITE);
    LCD.SetFont(&Font24);
    LCD.SetBackColor(LCD_COLOR_ORANGE); //Sets back to orange
    LCD.SetTextColor(LCD_COLOR_BLACK);


  while (1) {
    // Read RTC and display on LCD

    state_table[current_state]();
    userButton.fall(&IntButtonISR);
    ReadEEPROM(EEPROM_ADDR, eeprom_addr, data_read, 16);
    // LCD.SetFont(&Font8);
    // LCD.DisplayStringAt(0, 160, (uint8_t *)&data_read, CENTER_MODE);

    // puts(data_read);

    // puts(time1);
    // puts(time2);
    if(current_state != STATE2){button1.attach(&button1ISR, IRQ_FALL, 1000, true);}
    button3.attach(&button3ISR, IRQ_FALL, 1000, true);

    
    if(eewrite) {
        time_t press; 
        time(&press);
        char temp[8];
        strftime(temp, 8, "%H:%M:%S", localtime(&press));

         //string manipulation to get time1 and time2
        for(int i =0; i < 8; ++i){
            time1[i] = data_read[i];
            time2[i] = data_read[i+8];
        }

        for(int i =0; i < 8; ++i){
            writebuf[i] = time2[i];
            writebuf[i+8] = temp[i];
        }

        WriteEEPROM(EEPROM_ADDR, eeprom_addr, writebuf, 16);
        eewrite = false;
    }

    if(timeset){
        set_time(mktime(&t));
        hours = 0;
        minutes = 0;
        seconds = 0;
        timeset = false;
    }

    ThisThread::sleep_for(1000ms);
   
  }
}


// This function has 63 bytes write limit
void WriteEEPROM(int address, unsigned int eeaddress, char *data, int size) {
  char i2cBuffer[size + 2];
  i2cBuffer[0] = (unsigned char)(eeaddress >> 8);   // MSB
  i2cBuffer[1] = (unsigned char)(eeaddress & 0xFF); // LSB

  for (int i = 0; i < size; i++) {
    i2cBuffer[i + 2] = data[i];
  }

  int result = i2c.write(address, i2cBuffer, size + 2, false);
  thread_sleep_for(6);
}

void ReadEEPROM(int address, unsigned int eeaddress, char *data, int size) {
  char i2cBuffer[2];
  i2cBuffer[0] = (unsigned char)(eeaddress >> 8);   // MSB
  i2cBuffer[1] = (unsigned char)(eeaddress & 0xFF); // LSB

  // Reset eeprom pointer address
  int result = i2c.write(address, i2cBuffer, 2, false);
  thread_sleep_for(6);

  // Read eeprom
  i2c.read(address, data, size);
  thread_sleep_for(6);
}

