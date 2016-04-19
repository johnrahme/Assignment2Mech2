/*
 * File:   motor.c
 * Author: JohnMac
 *
 * Created on den 1 april 2016, 16:36
 */


#include <xc.h>
#include <math.h>
#include "motor.h"
#include "lcd.h"
#define CLOCKWISE 1 //Define clockwise and counter-clockwise
#define COUNTER_CLOCKWISE 0
#define STEPS_PER_ROTATION 48
#define DEG_PER_STEP 7.5 //Define step size
#define SPEED 20 //Define motor speed

void initializeMotor(){
    //Initialize motor to step 0 and write to LCD
    PORTC = halfSteps[0];
    lcdSetCursor(0x40);
    lcdWriteString("Steps:");
    lcdWriteToDigitBCD(nrOfSteps,4,1);
}

//Function moves half steps in the specified direction
void move(char steps, char direction){
    for(char i = 0; i<steps; i++){
        //Reset if clockwise
         if(cstep == 7 && direction == CLOCKWISE){
            PORTC = halfSteps[0];
            cstep = 0;
         }
         //Reset if counter-clockwise
         else if(cstep == 0 && direction == COUNTER_CLOCKWISE){
            PORTC = halfSteps[7];
            cstep = 7;
         }
         //Otherwise step in specified direction
         else if (cstep >= 0 && cstep <= 7){
             if(direction == CLOCKWISE){
                PORTC = halfSteps[cstep+1]; 
                cstep++;
             }
             else if(direction == COUNTER_CLOCKWISE){
                PORTC = halfSteps[cstep-1];
                cstep--;
             }
             
         }
         else{
               PORTC = 0x00;
         }
         
         //Write steps to LCD
         lcdSetCursor(0x40);
         lcdWriteString("steps:");
         if(direction==CLOCKWISE){
             nrOfSteps++;
             lcdWriteToDigitBCD(nrOfSteps,4,1);
         }
         else{
             nrOfSteps--;
             lcdWriteToDigitBCD(nrOfSteps,4,1);
         }     
        __delay_ms(SPEED);
    }
}

//Move the specified degree using the degree per step
void moveDeg(double deg){
    char stepsToMove;
    if(deg<0){
        stepsToMove = (char) round(-deg/(DEG_PER_STEP/2.0));
        move(stepsToMove,0);
    }
    else{
        stepsToMove = (char) round(deg/(DEG_PER_STEP/2.0)); 
        move(stepsToMove,1);
    }
}
