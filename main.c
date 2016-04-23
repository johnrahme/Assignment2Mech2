/*********************************************************************************
	Description: Controls stepper motor from debounced buttons
	Uses: DSX Experimenter board and SMPM
	Connections:
				iRobot - DSX Kit Addon Module

	Author: Kyle Alvarez

**********************************************************************************/
#include "common.h"
#include "ser.h"
#include "iRobot.h"
#include "timer0.h"


#pragma config BOREN = OFF, CPD = OFF, WRT = OFF, FOSC = HS, WDTE = OFF, CP = OFF, LVP = OFF, PWRTE = OFF

#define TMR0_VAL 100	//timer0 start count
#define DEG_PER_SEC 85;


// Interrupt service routine
void interrupt isr(void)
{
    //Serial interrupt handler
    ser_int();

    //Timer 1
    if(TMR0IF){
        TMR0IF = 0;
        TMR0 = TMR0_VAL;
        flashLed();
        squareTimers();
    }
}

void setup(void){
    __delay_ms(5000);   // Safety delay while iRobot serial buffers are streaming stuff
    ser_init(); 
      //Timer initialization
    initializeTimer0();
    setupIRobot();

    TRISB = 0;  //For heartbeat LED
}

void main (void){
    setup();
    char stopped = 0;
    LED1 = 0;
    //turnCW();
    drive();
    while(1){
        /*if(RTC_FLAG_90DEG&&stopped){
            drive();
            RTC_FLAG_90DEG = 0;
            stopped = 0;
        }
        if(RTC_FLAG_90DEG&&!stopped){
            turnCW();
            RTC_FLAG_90DEG = 0;
            stopped = 1;
        }*/
        if(RTC_FLAG_20000MS&&stopped){
            drive();
            RTC_FLAG_20000MS = 0;
            stopped = 0;
        }
        if(RTC_FLAG_20000MS&&!stopped){
            stop();
            RTC_FLAG_20000MS = 0;
            stopped = 1;
        }
    }

}
