#include "mbed.h"

InterruptIn userButton(BUTTON1);

DigitalOut led3(PG_13);
DigitalOut led4(PG_14);

Ticker flipper3;
Ticker flipper4;

Timeout t3;
Timeout t4;

Ticker c;

int state = 0; // default state on startup
int counter = 0;


void Flipper3ISR(){
    led3 = !led3;
}

void Flipper4ISR(){
    led4 = !led4;
}

void CounterISR(){
    counter++;

    if(state == 2){
        if(counter % 4 == 0){
            led4 = 1;
            led3 = 0;
        }
        else if (counter % 4 == 1){
            led4 = 0;
            led3 = 0;
        }
        else if(counter % 4 == 2){
            led4 = 0;
            led3= 1;
        }
        else if(counter % 4 == 3){
            led3 = 0;
            led4 = 0;
        }
    }
}

void delay3ISR() {flipper3.attach(&Flipper3ISR, 1s);}
void delay4ISR() {flipper4.attach(&Flipper4ISR, 1s);}


void UserButtonISR() {

    if(state != 1){
        led3 = 0;
        led4 = 1;
        flipper3.detach();
        flipper4.detach();
        t3.detach();
        t4.detach();
        c.detach();

        flipper3.attach(&Flipper3ISR, 1000ms);
        flipper4.attach(&Flipper4ISR, 1000ms);
        
        state = 1; 
    }
    else {
        flipper3.detach();
        flipper4.detach();
        t3.detach();
        t4.detach();

        led4 =1;
        led3 =0;
        counter = 0;

        c.attach(&CounterISR, 1000ms);



        state = 2;

    }
}

int main() {
    __enable_irq();
    if(state ==0){
        led3=0;
        led4=0;
        flipper4.attach(&Flipper4ISR,1000ms);
    }

    userButton.fall(&UserButtonISR);

  while (true) {
      
  }
}

