#include "common.h"
#include "iRobot.h"
#include "timer0.h"

char patternStage = 0;

void setupIRobot(void){
    ser_putch(START); //start create robot
    __delay_ms(5);
    ser_putch(FULL);
    __delay_ms(5);
}
//Drive straight forward
void drive(void){
    ser_putch(DRIVE); __delay_ms(5); ser_putch(0); __delay_ms(5); ser_putch(200); __delay_ms(5); ser_putch(127); __delay_ms(5); ser_putch(255);__delay_ms(5);
}
//Turn clockwise 
void turnCW(){
    ser_putch(DRIVE); __delay_ms(5); ser_putch(0); __delay_ms(5); ser_putch(200); __delay_ms(5); ser_putch(255); __delay_ms(5); ser_putch(255);__delay_ms(5);
}
void stop(void){
    ser_putch(DRIVE); __delay_ms(5); ser_putch(0); __delay_ms(5); ser_putch(0); __delay_ms(5); ser_putch(0); __delay_ms(5); ser_putch(0);__delay_ms(5);
}

void moveDistanceForward(int centimeters){
    RTC_MOVE_PATTERN_COUNTER = 0; //Reset the counter
    // 21053/4/100 = 52.6325 ---> milliseconds to move one centimeter
    int timeToMoveOneCentimeter = 53;
    int totalTimeToMove = centimeters*timeToMoveOneCentimeter;
    //Set the time for the counter to wait until next step in pattern
    MOVE_PATTERN_TIME = totalTimeToMove;
    drive();
    LED0 = 0;
    LED1 = 1;
}
void turnDegreesCW(int degrees){
    RTC_MOVE_PATTERN_COUNTER = 0; //Reset the counter
    // 1130/90 = 12.55 ---> milliseconds to move one degree
    int timeToMoveOneDegree = 13;
    int totalTimeToTurn = degrees*timeToMoveOneDegree;
    //Set the time for the counter to wait until next step in pattern
    MOVE_PATTERN_TIME = totalTimeToTurn;
    turnCW();
    LED0 = 1;
    LED1 = 0;
    
}
char moveSquarePattern(){
    
    if(patternStage == 0||(RTC_FLAG_MOVE_PATTERN&&patternStage == 2)||(RTC_FLAG_MOVE_PATTERN&&patternStage == 4)||(RTC_FLAG_MOVE_PATTERN&&patternStage == 6)){
        //Move forward 100 cm
        moveDistanceForward(100);
        //increment pattern stage
        patternStage++;
        //Reset Pattern Flag
        RTC_FLAG_MOVE_PATTERN = 0;
    }
    else if((RTC_FLAG_MOVE_PATTERN&&patternStage == 1)||(RTC_FLAG_MOVE_PATTERN&&patternStage == 3) || (RTC_FLAG_MOVE_PATTERN&&patternStage == 5)){
        //Move forward 100 cm
        turnDegreesCW(90);
        //increment pattern stage
        patternStage++;
        //Reset Pattern Flag
        RTC_FLAG_MOVE_PATTERN = 0;
    }
    else if (RTC_FLAG_MOVE_PATTERN&&patternStage == 7){
        //End of pattern stop the motor
        stop();
        //Reset pattern stage
        patternStage = 0;
        RTC_FLAG_MOVE_PATTERN = 0;
        
        LED0 = 0;
        LED1 = 0;
        return 1;
    }
    return 0;
}
