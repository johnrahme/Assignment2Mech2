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
#include "lcd.h"
#include "adConv.h"


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
        //User timer0 source function to flash a led
        //flashLed();
        //Increment counter for the current pattern
        updateMovePattern();
        //Debounce the buttons
        debounceButtons();
    }
    if(ADIF){
        conversionDone = 1; //Set conversion done flag
        ADIF = 0;
    }
}

void setup(void){
    __delay_ms(5000);   // Safety delay while iRobot serial buffers are streaming stuff
    ser_init(); 
      //Timer initialization
    initializeTimer0();
    setupIRobot();
    // Set RB0-RB3 as pushbuttons and RB4-RB5 as LED:S
    TRISB = 0b00001111;  //For LED:s and pushbuttons
    initializeADC();
    setupLCD();//THIS MIGHT FK UP THE ROBOT FROM MOVING DONT KNOW YET, GOTTA TRY IT
}

void main (void){
    setup();
    int distanceTraveled = 0;
    LED1 = 0;
    LED0 = 0;
    
    //lcdSetCursor(0x40);
    //lcdWriteString("I am iRobot!");
    
    char patternDone = 1;
    char squarePatternDone = 1;
    char straightPatternDone = 1;
    
    startADCConversion();
    while(1){
        //Check ADC coversion
        if(conversionDone){ //Check conversion done flags
            conversionDone = 0; 
            printADCData(); //Prints the conversion data to the LCD
        }
        
        //Start the square pattern if PB0 is pressed
        if(pb0Pressed){
            distanceTraveled = 0; //added in to 0 the total distance traveled at the start of the function
            squarePatternDone = 0;
            patternDone = 0;
            pb0Pressed = 0;
        }
        
        if(pb1Pressed){
            distanceTraveled = 0; //added in to 0 the total distance traveled at the start of the function
            straightPatternDone = 0;
            patternDone = 0;
            pb1Pressed = 0;
        }
        
              
            //If square pattern is not done update it
        if(!patternDone&&!squarePatternDone){
            //Square pattern function returns an a 0 if its not done and a 1 if it is done
            
            squarePatternDone = moveSquarePattern();
            patternDone = squarePatternDone;
        }
        if(!patternDone&&!straightPatternDone){
            //Square pattern function returns an a 0 if its not done and a 1 if it is done
            straightPatternDone = moveStraightPattern();
            patternDone = straightPatternDone;
        }
        
        //Update the LCD with the distance travelled
        //distanceTraveled += getTraveledDistance(); // Freezes program if not connected to robot 
        lcdSetCursor(0x40);
        lcdWriteToDigitBCD(distanceTraveled, 4, 0);
    }

}
