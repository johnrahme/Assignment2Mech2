#include "scanner.h"
#include "motor.h"
#include "adConv.h"
#include "timer0.h"

void setScannerSpeed(char speed){
    SM_COUNTER_TIME = speed;
}
char updateScanner(){
    //First rotate and store closes wall
    if(RTC_MOVE_SM_FLAG&&scanRunning){
        RTC_MOVE_SM_FLAG = 0;
        if(scanStepNumber<stepsToMove){
            scanStepNumber++;
            
            move(0);
            if(smallestDistance > latestReadMeterValue){
                smallestDistance = latestReadMeterValue;
                smallestValueStep = scanStepNumber;
            }
        }
        else{
            scanStepNumber = 0;
            scanRunning = 0;
            movingToWall = 1;
        }
        
    }
    //Then rotate back
    if(movingToWall&&RTC_MOVE_SM_FLAG){
        RTC_MOVE_SM_FLAG;
        scanStepNumber++;
        if(scanStepNumber<(stepsToMove-smallestValueStep)){
            move(1);
        }
        else{
            movingToWall = 0;
            scanStepNumber = 0;
            return 1;
        }
    }
    return 0;
    
}
