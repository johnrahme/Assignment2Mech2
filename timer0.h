#ifndef TIMER0_H
#define TIMER0_H
#include <xc.h>
#include "common.h"

volatile unsigned int rtcCounter = 0;

volatile bit pb0Pressed = 0; // Bit flag to signal if the pushbutton is pressed
volatile bit pb0Released = 0; // Bit to signal if the pushbutton has been released
volatile unsigned char pbCounter = 0; //Button counter

volatile bit pb1Pressed = 0; 
volatile bit pb1Released = 0; 
volatile unsigned char pbCounter2 = 0; 

volatile bit pb2Pressed = 0; 
volatile bit pb2Released = 0; 
volatile unsigned char pbCounter3 = 0;

volatile bit pb3Pressed = 0;
volatile bit pb3Released = 0; 
volatile unsigned char pbCounter4 = 0;

volatile bit updateLcdIRData = 0; //Update refresh bit flag
volatile unsigned char irLcdCounter = 0; //LCD counters

volatile bit RTC_FLAG_MOVE_PATTERN = 0;
volatile unsigned int RTC_MOVE_PATTERN_COUNTER = 0;
volatile unsigned int MOVE_PATTERN_TIME = 0;

volatile bit RTC_MOVE_SM_FLAG = 0;
volatile unsigned char RTC_MOVE_SM_COUNTER = 0;
volatile char SM_COUNTER_TIME = 30;
       
volatile bit RTC_FLAG_90DEG = 0;
volatile unsigned int RTC_90DEG_COUNTER = 0;

volatile bit RTC_FLAG_20000MS = 0;
volatile unsigned int RTC_20000MS_COUNTER = 0;


void initializeTimer0();
void debounceButtons();
void lcdRefresh();
void flashLed();
void updateMovePattern();
void stepperMotorCounter();


#endif TIMER0_H