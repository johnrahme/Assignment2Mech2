#include "scanner.h"
#include "motor.h"
#include "adConv.h"
#include "timer0.h"
#include "lcd.h"

void setScannerSpeed(char speed){
    SM_COUNTER_TIME = speed;
}
char updateScanner(){
    
    //First rotate and store closes wall
    if(RTC_MOVE_SM_FLAG&&scanRunning){
        RTC_MOVE_SM_FLAG = 0;
        if(scanStepNumber < stepsToMove){                               //if scanner step number is less than steps to move
            scanStepNumber++;                                        //increment scanner step number
            move(0);                                                    //move in direction 0(clockwise)
            if(latestReadMeterValue < smallestDistance){                //if smallest recorded distance is greater than distance just read by scanner;
                smallestDistance = latestReadMeterValue;                //last read distance becomes the new mallest distance read
                smallestValueStep = scanStepNumber;                     //smallest step number changes accordingly to match distance just scanned
            }
        }
        else{                                                           //otherwise;
            scanStepNumber = 0;                                         //clear scanner step number
            scanRunning = 0;                                            //clear scan running
            movingToWall = 1;                                        //robot rotates to face wall
            stepsFromOrigin = 0;
        }
        
    }
    //Then rotate back
    if(movingToWall&&RTC_MOVE_SM_FLAG){                                 //if scanner is rotating to face wall and stepper motor flag is raised
        RTC_MOVE_SM_FLAG=0;                                             //clear flag
        scanStepNumber++;                                               //increment scanner/stepper motor step number
        if(scanStepNumber<(stepsToMove-smallestValueStep)){             //if scanner/stepper motor step number is less than the difference between steps required to move and the smallest stepper value
            move(1);                                                    //move in direction 1(counter-clockwise))
        }
        else{                                                    //otherwise;
            stepsFromOrigin = smallestValueStep;
            movingToWall = 0;                                           //do not rotate scanner to face wall
            scanStepNumber = 0;                                    //reset scanner/stepper motor step number value
            smallestDistance = 20000;
            smallestValueStep = 0;
            return 1;                                                   //return 1(meaning sequence is complete)
        }
    }
    
    return 0;                                                           //return 0(meaning sequence is incomplete)
    
}

//Ignore this for now, just a test that is not working
void updateScannerOldWay(){
    
    //First rotate and store closes wall
    for(int i = 0; i<400; i++){
         move(0);
        if(conversionDone){ //Check conversion done flags
            conversionDone = 0; 
            printADCData(); //Prints the conversion data to the LCD
        }                                                        //move in direction 0(clockwise)
        if(latestReadMeterValue < smallestDistance){                //if smallest recorded distance is greater than distance just read by scanner;
                smallestDistance = latestReadMeterValue;                //last read distance becomes the new mallest distance read
                smallestValueStep = scanStepNumber;                     //smallest step number changes accordingly to match distance just scanned
        } 
    }
    scanStepNumber = 0;                                         //clear scanner step number
    scanRunning = 0;                                            //clear scan running                                    //robot rotates to face wall
    stepsFromOrigin = 0;
    //Then rotate back
    for(int i = 0; i<(400-smallestValueStep); i++){                                           //if scanner/stepper motor step number is less than the difference between steps required to move and the smallest stepper value
            move(1);                                                    //move in direction 1(counter-clockwise))
                                               //otherwise;
            stepsFromOrigin = smallestValueStep;                                          //do not rotate scanner to face wall                                 //reset scanner/stepper motor step number value
            smallestDistance = 20000;
            smallestValueStep = 0;
    }
                                                        //return 0(meaning sequence is incomplete)    
}
//Just to move the motor back to its starting position
void resetToOrigin(){
    moveOld(stepsFromOrigin,1);
    stepsFromOrigin = 0;
}
