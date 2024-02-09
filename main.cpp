#include "LCD_DISCO_F429ZI.h"
#include "mbed.h"
#include <chrono>
#include <climits>
#include <cstdint>

#define DEBOUNCE_PATTERN 0x7FFF

#define LCD_WIDTH 240
#define LCD_HEIGHT 320
using namespace std::chrono;


LCD_DISCO_F429ZI LCD;

InterruptIn userButton(BUTTON1);
InterruptIn ext_button(PG_1, PullUp);

DigitalOut led3(PG_13);

Ticker Flipper3;
Timer react_time; 

int latest;
uint32_t best = 0xFFFF;

int state = 0; 
int counter; 
bool print_flag =false;

bool state_change = false;
bool cheating = false;
bool reset = false;

uint16_t shiftregister;

//variable delay
Ticker c; 
int countstate=0; 


void Flipper3ISR(){
    led3 = !led3;
}

typedef enum {STATE0 = 0 , STATE1 = 1, STATE2 = 2} State_Type; 
State_Type current_state = STATE0;



void ledTrigger(){
    if(countstate ==1){
        led3 = 1;
        react_time.start();
        c.detach();
        countstate = 0;
    }
}


void state0(){
    led3 = 0;
    Flipper3.detach();
    Flipper3.attach(&Flipper3ISR, 100ms);
}

void state1(){
    led3 = 0;
    Flipper3.detach();
    counter = rand() % 5 + 1;
    
    //interupt-based delay
    countstate = 1;
    c.attach(&ledTrigger, counter*1000ms);
}

void state2(){
    
    react_time.stop();
    latest = duration_cast<milliseconds>(react_time.elapsed_time()).count();
    if(led3 != 1){
        current_state = STATE0;
        c.detach();
        countstate = 0;
        cheating = true;
        return;
    }

    if(latest < best){
        best = latest;
    }
    print_flag = true;
    react_time.reset();

}

void (*state_table[])() = {state0, state1, state2};



void stateChange(){
    if(current_state == STATE1){
        current_state = STATE2;
    }
    else{
        current_state = STATE1;
    }
    state_change =true;

}


void update_lcd() {
  LCD.Clear(LCD_COLOR_WHITE);
  uint8_t text[30];
  uint8_t besttext[30];
  LCD.DisplayStringAt(0, 40, (uint8_t *)"Reaction Times", CENTER_MODE);
  sprintf((char *)text, "Latest:%d ms", latest);
  LCD.DisplayStringAt(0, 80, (uint8_t *)&text, CENTER_MODE);
  sprintf((char *) besttext, "Best: %d ms", best);
  LCD.DisplayStringAt(0, 120, (uint8_t *)&besttext, CENTER_MODE);
}



void extButtonISR(){
    current_state = STATE0;
    LCD.Clear(LCD_COLOR_WHITE);
    latest = 0; 
    best = 0xFFFF; 
    reset = true;
}





int main() {
    srand(time(NULL));
    LCD.SetFont(&Font24);
    LCD.SetTextColor(LCD_COLOR_DARKBLUE);

    Flipper3.attach(&Flipper3ISR, 100ms);

    while (1) {
        if(state_change){
            state_table[current_state]();
            state_change =false;
            if(cheating){
                state_table[current_state]();
                cheating = false;
            }
        }
        userButton.fall(&stateChange);
        // ThisThread::sleep_for(20ms);

        if(print_flag){
            // printf("%d",latest);
            update_lcd();
            print_flag = false;

        }
        shiftregister = (shiftregister << 1) + ext_button.read();

        if(shiftregister == DEBOUNCE_PATTERN){
            extButtonISR();
            state_table[current_state]();
            reset = false;
        }
    }
}
