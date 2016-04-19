#include <xc.h>
#include "motor.h"
#include "timer0.h"
#include "adConv.h"
#include "lcd.h"
#include "common.h"

#pragma config BOREN = OFF, CPD = OFF, WRT = OFF, FOSC = HS, WDTE = OFF, CP = OFF, LVP = OFF, PWRTE = OFF //for XC8 compiler
#define TMR0_VAL 100 //Used to make timer 0 run every ms

bit motorToggle = 0;

void interrupt isr(void){
    if (T0IF){ // If Timer 0 overflow occurred
        T0IF = 0; // Clear flag
        TMR0 = TMR0_VAL; // Set back to start value 
        flashLed(); //Check if led should flash
        debounceButtons(); //Debounce buttons 1 to 4
        lcdRefresh(); // Refresh the lcd
    }
    if(ADIF){
        conversionDone = 1; //Set conversion done flag
        ADIF = 0;
    }
}
void initialise (void){

    //Initialize the B and C ports
    TRISB = 0b00001111;
    TRISC = 0;
    
    //Set LED:S to off
    LED0 = 1;
    LED1 = 1;
    
    //Initialize all components
    initializeTimer0();
    initializeADC();
    setupLCD();
    initializeMotor();

    
    
}

void main (void) {
    //initialise the program
    initialise();
    //Begin converting ADC
    startADCConversion();
    while(1){
        

        if(conversionDone){ //Check conversion done flags
            conversionDone = 0;
            int result = readADCData(); // Read data from ADC
            int resultInMeters = readADCMeter(); //Read data and convert to meters
            if(updateLcdIRData){ // Check LCD refresh rate and print to LCD
                lcdSetCursor(0x00);
                lcdWriteString("Raw:");
                lcdWriteToDigitBCD(result,4,0);
                lcdSetCursor(0x09);
                lcdWriteString("=>");
                lcdWriteToDigitBCD(resultInMeters,3,0);
                lcdWriteString("cm");
                updateLcdIRData = 0;
            }
            // Restart the ADC conversion
            startADCConversion();
            
        }
        //Move motor one step clockwise when pb0 is pushed
        if(pb0Pressed){
            move(1,1);
            LED0 = !LED0;
            pb0Pressed = 0;
        }
        //Move motor one step counter-clockwise when pb1 is pushed
        if(pb1Pressed){
            move(1,0);
            pb1Pressed = 0;
        }
        //Toggle motor 180 degrees when pb2 is pushed
        if(pb2Pressed){
            if(motorToggle){
                moveDeg(-180);
            }
            else{
                moveDeg(180);
            }
            motorToggle = !motorToggle;
            pb2Pressed = 0;
        }
        //Toggle led0 when pb3 is pushed
        if(pb3Pressed){
            LED0 = !LED0;
            pb3Pressed = 0;
        }

        __delay_ms(20);
    }
}