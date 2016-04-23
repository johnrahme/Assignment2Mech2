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


#pragma config BOREN = OFF, CPD = OFF, WRT = OFF, FOSC = HS, WDTE = OFF, CP = OFF, LVP = OFF, PWRTE = OFF

#define TMR0_VAL 100	//timer0 start count
#define HBLED RB0

volatile bit RTC_FLAG_500MS = 0;

//global variables
volatile unsigned int RTC_Counter = 0;

// Interrupt service routine
void interrupt isr(void)
{
    //Serial interrupt handler
    ser_int();

    //Timer 1
    if(TMR0IF){
        TMR0IF = 0;
        TMR0 = TMR0_VAL;

        RTC_Counter++;
        //set clock flags
        if(RTC_Counter % 500 == 0) {
            RTC_FLAG_500MS = 1;
            RTC_Counter = 0;	//reset RTC Counter
        }
    }
}

void setup(void){
    __delay_ms(5000);   // Safety delay while iRobot serial buffers are streaming stuff
    ser_init();
    PEIE = 1;
    GIE = 1;
    setupIRobot();

    TRISB = 0;  //For heartbeat LED
}

void main (void){
    setup();
    drive();
    while(1){
        if(RTC_FLAG_500MS){
            HBLED = !HBLED;
        }
    }

}
