#include "mbed.h"
#include "LCD_DISCO_F429ZI.h"
#include "DebouncedInterrupt.h"

/*  Angular Resolution: 360/48 = 7.5 degrees/step
Student1: Dhilan Teeluckdharry, 400434738, Period is 38
Full Step: 38/48 = 0.792 s/step
Half Step: 38/96 = 0.396 s/step

Student2: Leon Yao, 400269612, Period is 45
Full Step: 45/48 = 0.9375 s/step
Half Step: 45/96 = 0.469 s/step
*/

// main() runs in its own thread in the OS

#define NUM_STEP_F 4
#define NUM_STEP_H 7

LCD_DISCO_F429ZI LCD;

DigitalOut red(PB_0); //In1
DigitalOut gray(PE_8); //In2
DigitalOut yellow(PE_12); //In3
DigitalOut black(PE_14); //In4


DebouncedInterrupt External1(PF_9);
DebouncedInterrupt External2(PA_6);
DebouncedInterrupt External3(PA_5);
DebouncedInterrupt External4(PA_7);

bool isFull = true;
bool isClockwise = true;


std::chrono::system_clock::duration full_ss;
std::chrono::system_clock::duration half_ss;
std::chrono::system_clock::duration speed_ms;



bool student1_flag = false; char name1[] = "Dhilan Teeluckdharry"; int number1 = 400434738;  int secs1 = 38;
bool student2_flag = false; char name2[] = "Leon Yao"; int number2 = 400269612; int secs2 = 45;
int period;
char name[45];
char number[30];
char secs[30];

InterruptIn userButton(BUTTON1);
Ticker stepper;

int student=0;


void Step1_f(){
    red = 1;
    gray = 0;
    yellow = 1;
    black = 0;
}

void Step2_f(){
    red = 1;
    gray = 0;
    yellow = 0;
    black = 1;
}


void Step3_f(){
    red = 0;
    gray = 1;
    yellow = 0;
    black = 1;
}

void Step4_f(){
    red = 0;
    gray = 1;
    yellow = 1;
    black = 0;
}



void Step1_h(){
    red = 1;
    gray = 0;
    yellow = 1;
    black = 0;
}

void Step2_h(){
    red = 1;
    gray =0;
    yellow = 0;
    black = 0;
}

void Step3_h(){
    red = 1;
    gray =0;
    yellow = 0;
    black = 1;
}

void Step4_h(){
    red=0;
    gray=0;
    yellow = 0;
    black=1;
}

void Step5_h(){
    red=0;
    gray=1;
    yellow=0;
    black=1;
}

void Step6_h(){
    red = 0;
    gray=1;
    yellow=0;
    black = 0;
}

void Step7_h(){
    red = 0;
    gray=1;
    yellow=1;
    black=0;
}

void (*state_table_f[])() = {Step1_f, Step2_f, Step3_f, Step4_f};
int cur_step_f = 0;


void(*state_table_h[])() = {Step1_h, Step2_h, Step3_h, Step4_h, Step5_h, Step6_h, Step7_h};
int cur_step_h = 0;


void stepperISR(){
    if(isFull){

        if(isClockwise){
            cur_step_f = ++cur_step_f % NUM_STEP_F;
            state_table_f[cur_step_f]();
        }
        else{
            cur_step_f = ++cur_step_f % NUM_STEP_F;
            state_table_f[(NUM_STEP_F-1)-cur_step_f]();
        }

    }
    else{
        if(isClockwise){
            cur_step_h = ++cur_step_h % NUM_STEP_H;
            state_table_h[cur_step_h]();
        }
        else{
            cur_step_h = ++cur_step_h % NUM_STEP_H;
            state_table_h[(NUM_STEP_H-1)-cur_step_h]();
        }
    }
}


void state0(){
}

void state1(){
    speed_ms = 38000ms;
    full_ss = speed_ms/48;
}

void state2(){
    speed_ms = 45000ms;
    full_ss = speed_ms/48;

}

typedef enum {STATE0 = 0, STATE1 = 1, STATE2 = 2} State_Type;
State_Type current_state = STATE0;
void (*state_table[])() = {state0, state1, state2};

int counter = 0;



void userButtonISR(){

    if(student == 1){
        student = 2;
        current_state = STATE2;
        period = 45;
        speed_ms = 45000ms;
        full_ss = speed_ms/48;
        half_ss = speed_ms/96;
        stepper.detach();
        if(isFull){
            stepper.attach(&stepperISR, full_ss);
        }
        else{
            stepper.attach(&stepperISR, half_ss);
        }
        student1_flag = false; 
        student2_flag = true;  
    }
    else{
        student = 1;
        current_state = STATE1;
        speed_ms = 38000ms;
        period=38;
        full_ss = speed_ms/48;
        half_ss = speed_ms/96;
        stepper.detach();
        if(isFull){
            stepper.attach(&stepperISR, full_ss);
        }
        else{
            stepper.attach(&stepperISR, half_ss);
        }
        student1_flag = true;
        student2_flag = false;
    }

}


void External1ISR(){
    stepper.detach();
    if(isFull){
        stepper.attach(&stepperISR, half_ss);
    }
    else{
        stepper.attach(&stepperISR, full_ss);
    }
    cur_step_f = 0;
    cur_step_h = 0;
    isFull = !isFull;
}


void External2ISR(){
    isClockwise = !isClockwise;
    cur_step_f = 0;
    cur_step_h = 0;
}


void IncSpeed(){
    period++;
    speed_ms += 1000ms;
    full_ss = speed_ms/48;
    half_ss = speed_ms/96;
    stepper.detach();
    if(isFull){
        stepper.attach(&stepperISR, full_ss);
    }
    else{
        stepper.attach(&stepperISR, half_ss);
    }

}

void DecSpeed(){
    period--;
    speed_ms -= 1000ms;
    full_ss = speed_ms/48;
    half_ss = speed_ms/96;
    stepper.detach();
    if(isFull){
        stepper.attach(&stepperISR, full_ss);    
    }
    else{
        stepper.attach(&stepperISR, half_ss);
    }
}




int main()
{

    LCD.SetFont(&Font16);
    LCD.SetTextColor(LCD_COLOR_BLACK);
    LCD.SetBackColor(LCD_COLOR_WHITE);

    userButton.fall(&userButtonISR);
    External1.attach(&External1ISR, IRQ_FALL, 100, false);
    External2.attach(&External2ISR, IRQ_FALL, 100, false);
    External3.attach(&IncSpeed, IRQ_FALL, 100, false);
    External4.attach(&DecSpeed, IRQ_FALL, 100, false);
    // yellow = 1;

    while (true) {
        LCD.Clear(LCD_COLOR_WHITE);


        if(student1_flag || student2_flag){

            if(student1_flag){

                sprintf(name, "%s", name1);
                sprintf(number, "%d", number1);
                sprintf(secs, "Period: %d", period);

                LCD.DisplayStringAt(0,80, (uint8_t*)name, CENTER_MODE);
                LCD.DisplayStringAt(0, 100, (uint8_t*)number, CENTER_MODE);
                LCD.DisplayStringAt(0,120, (uint8_t*)secs, CENTER_MODE);

            }

            if(student2_flag){

                sprintf(name, "%s", name2);
                sprintf(number, "%d", number2);
                sprintf(secs, "Period: %d", period);

                LCD.DisplayStringAt(0,80, (uint8_t*)name, CENTER_MODE);
                LCD.DisplayStringAt(0, 100, (uint8_t*)number, CENTER_MODE);
                LCD.DisplayStringAt(0,120, (uint8_t*)secs, CENTER_MODE);
            }

            char stepmode[40];
            if(isFull){
                sprintf(stepmode, "Step mode: Full");
            }
            else{
                sprintf(stepmode, "Step mode: Half");
            }
            LCD.DisplayStringAt(0, 160, (uint8_t *)stepmode, CENTER_MODE);

        }

        // ThisThread::sleep_for(10ms);
        // ThisThread::sleep_for(100ms);

    }
}

