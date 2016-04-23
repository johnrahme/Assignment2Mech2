
#include <xc.h>
#include "timer0.h"
#define DEBOUNCE_REQ_COUNT 10 // Debounce for 10 ms
#define LCD_REFRESH_RATE 50 // set refresh rate 

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

void squareTimers(){
    
    RTC_90DEG_COUNTER++;
      if(RTC_90DEG_COUNTER % 1130 == 0){
            RTC_FLAG_90DEG = 1;
            RTC_90DEG_COUNTER = 0;
      }
    RTC_20000MS_COUNTER++;
      if(RTC_20000MS_COUNTER % 20000 == 0){
            RTC_FLAG_20000MS = 1;
            RTC_20000MS_COUNTER = 0;
      }
}

//Debounce buttons SB2, SB3, SB4 and SB5
void debounceButtons(){

        if(PB0) {
            pbCounter++; //Increment counter
            if(pbCounter >DEBOUNCE_REQ_COUNT &&pb0Released){
                //Set flags
                pb0Pressed = 1;
                pb0Released = 0;
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
