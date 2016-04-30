
#include <xc.h>
#include "timer0.h"
#define DEBOUNCE_REQ_COUNT 10 // Debounce for 10 ms
#define LCD_REFRESH_RATE 200 // set refresh rate 

void initializeTimer0(){
        
    //Timer initialization
    TMR0 = 100;
    T0CS = 0;
    T0SE = 0;
    //Set 32 prescalar
    PSA = 0;
    PS0 = 0;
    PS1 = 0;
    PS2 = 1;
    //Enable timer interrupts
    TMR0IE = 1;
    ei();
}

//Set flag for refreshing LCD
void lcdRefresh(){
    irLcdCounter++;
    if(irLcdCounter==LCD_REFRESH_RATE){
        updateLcdIRData = 1;
        updateSensorsFlag = 1;
        irLcdCounter = 0;
    }
    
}

//Set flag for flashing led at 1HZ
void flashLed(){
    rtcCounter++;
    if(rtcCounter == 500){
        LED1 = !LED1;
        rtcCounter = 0;
    }
}

void updateMovePattern(){
    
    
    RTC_MOVE_PATTERN_COUNTER++;
      if(RTC_MOVE_PATTERN_COUNTER == MOVE_PATTERN_TIME){
            RTC_FLAG_MOVE_PATTERN = 1;
            RTC_MOVE_PATTERN_COUNTER = 0;
      }
    RTC_LOST_WALL_COUNTER++;
    if(RTC_LOST_WALL_COUNTER == LOST_WALL_TIME){
        RTC_LOST_WALL_FLAG = 1;
        RTC_LOST_WALL_COUNTER = 0;
    }
}


void stepperMotorCounter(){
    
    RTC_MOVE_SM_COUNTER++;
    if(RTC_MOVE_SM_COUNTER == SM_COUNTER_TIME){
        RTC_MOVE_SM_FLAG = 1;
        RTC_MOVE_SM_COUNTER = 0;
    }
}

//Debounce buttons SB2, SB3, SB4 and SB5
void debounceButtons(){

        if(PB0) {
            pbCounter++; //Increment counter
            if(pbCounter >DEBOUNCE_REQ_COUNT &&pb0Released){
                //Set flags
                pb0Pressed = 1;                                 //Raise Pushbutton0 Pressed flag
                pb0Released = 0;                                //Clear Pushbutton0 Released flag
            }
        }
        else{
            //Set flags
            pbCounter = 0;
            pb0Released = 1;
        }
    
    if(PB1) {
            pbCounter2++;
            if(pbCounter2 >DEBOUNCE_REQ_COUNT &&pb1Released){
                pb1Pressed = 1;
                pb1Released = 0;
            }
        }
        else{
            pbCounter2 = 0;
            pb1Released = 1;
        }
        if(PB2) {
            pbCounter3++;
            if(pbCounter3 >DEBOUNCE_REQ_COUNT &&pb2Released){
                pb2Pressed = 1;
                pb2Released = 0;
            }
        }
        else{
            pbCounter3 = 0;
            pb2Released = 1;
        }
        if(PB3) {
            pbCounter4++;
            if(pbCounter4 >DEBOUNCE_REQ_COUNT &&pb3Released){
                pb3Pressed = 1;
                pb3Released = 0;
            }
        }
        else{
            pbCounter4 = 0;
            pb3Released = 1;
        }        
}
