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
#include "SPI.h"
#define CLOCKWISE 1 //Define clockwise and counter-clockwise
#define COUNTER_CLOCKWISE 0
#define DEG_PER_STEP 1.8 //Define step size
#define SPEED 20 //Define motor speed

void initializeMotor(){
    //Initialize motor to step 0 and write to LCD
    PORTC = halfSteps[0];
    lcdSetCursor(0x40);
    lcdWriteString("Steps:");
    lcdWriteToDigitBCD(nrOfSteps,4,1);
}

//Set the motor to move steps in a specific direction
void moveOld(char steps, char direction){
    if(direction == CLOCKWISE){
        // USes she SPI to set the motor to CW
        setToMotorCW();
    }
    else if(direction == COUNTER_CLOCKWISE){
        // USes she SPI to set the motor to CCW
        setToMotorCCW();
    }
   for(int i = 0; i<steps; i++){
        SM_STEP();
        __delay_ms(SPEED);
   }
}
void move(char direction){
    if(direction == CLOCKWISE){
        // USes she SPI to set the motor to CW
        setToMotorCW();
    }
    else if(direction == COUNTER_CLOCKWISE){
        // USes she SPI to set the motor to CCW
        setToMotorCCW();
    }
    SM_STEP();
}

//Move the specified degree using the degree per step
void moveDeg(double deg){
    char stepsToMove;
    if(deg<0){
        stepsToMove = (char) round(-deg/(DEG_PER_STEP/2.0));
        moveOld(stepsToMove,0);
    }
    else{
        stepsToMove = (char) round(deg/(DEG_PER_STEP/2.0)); 
        moveOld(stepsToMove,1);
    }
}


void moveMotorCont(void){
    time_count++;
    if(time_count % 1 == 0) {
            //FLAG_10MS = 1;
            SM_STEP();
            time_count = 0;
     }
}