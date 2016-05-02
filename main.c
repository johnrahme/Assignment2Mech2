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
#include "SPI.h"
#include "motor.h"
#include "adConv.h"
#include "patternHandler.h"
#include "scanner.h"

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
        flashLed();
        //Increment counter for the current pattern
        updateMovePattern();
        //Debounce the buttons
        debounceButtons();
        stepperMotorCounter();
        lcdRefresh();
        
        //moveMotorCont();
    }
    if(ADIF){
        conversionDone = 1; //Set conversion done flag
        ADIF = 0;
    }
}

void setup(void){
    __delay_ms(5000);   // Safety delay while iRobot serial buffers are streaming stuff
    setupSPI();
    ser_init(); 
      //Timer initialization
    initializeTimer0();
    setupIRobot();
    // Set RB0-RB3 as pushbuttons and RB4-RB5 as LED:S
    TRISB = 0b00001111;  //For LED:s and pushbuttons  
    
    //Set SPI to motor
    setToMotorCW();
    setupLCD();//THIS MIGHT FK UP THE ROBOT FROM MOVING DONT KNOW YET, GOTTA TRY IT
    initializeADC();
}

void main (void){
    setup();
    LED1 = 0;
    LED0 = 0;
    
    
    startADCConversion();
    while(1){   
        //move(10,0);
        //Check ADC coversion
        if(conversionDone){ //Check conversion done flags
            conversionDone = 0; 
            printADCData(); //Prints the conversion data to the LCD
        }
        
        //Start the square pattern if PB0 is pressed
        if(pb0Pressed){
            distanceTraveled = 0; //added in to 0 the total distance traveled at the start of the function 
            squarePatternDone = 0;
            //followWallPatternStart = 1;
            //turnAndDriveDirect(100,200);
            patternDone = 0;
            pb0Pressed = 0;
        }
        
        if(pb1Pressed){
            distanceTraveled = 0; //added in to 0 the total distance traveled at the start of the function
            straightPatternDone = 0;
            patternDone = 0;
            pb1Pressed = 0;
        }
        if(pb2Pressed){
            setScannerSpeed(8);
            scanRunning = 1;
            onlyScan = 1;
            //moveOld(100,0);
            //moveOld(100,0);
            pb2Pressed = 0;
        }
        if(pb3Pressed){
            setScannerSpeed(8);
            scanRunning = 1;
            distanceTraveled = 0;
            onlyScan = 0;
            pb3Pressed = 0;
        }     
        
        //Use patternHandler to update the patterns
        updatePatterns();
        
        //Update the LCD with the distance travelled and check bumper sensors
        if(!scanRunning&&!movingToWall){
            //Update the LCD with the distance travelled and check bumper sensors
            updateSensors();// LOOK! Freezes program if not connected to robot
        }
        
    }
    

}
