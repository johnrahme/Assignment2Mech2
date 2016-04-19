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

void initializeTimer0();
void debounceButtons();
void lcdRefresh();
void flashLed();

#endif TIMER0_H