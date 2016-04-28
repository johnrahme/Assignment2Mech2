#include "common.h"
#include "iRobot.h"
#include "timer0.h"
#include "lcd.h"
#include "adConv.h"

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

void turnAndDrive(int radius){
    char highByte = 0;
    char lowByte = 0;
    if(radius <0){
        highByte = -radius/256;
        lowByte = -radius%256;
        highByte = highByte ^ 0b11111111;
        lowByte = lowByte ^ 0b11111111;
    }
    else{
        highByte = radius/256;
        lowByte = radius%256;
    }
    ser_putch(DRIVE); __delay_ms(5); ser_putch(0); __delay_ms(5); ser_putch(200); __delay_ms(5); ser_putch(highByte); __delay_ms(5); ser_putch(lowByte);__delay_ms(5);
}

//Turn clockwise 
void turnCW(){
    ser_putch(DRIVE); __delay_ms(5); ser_putch(0); __delay_ms(5); ser_putch(200); __delay_ms(5); ser_putch(255); __delay_ms(5); ser_putch(255);__delay_ms(5);
}
void turnCCW(){
    ser_putch(DRIVE); __delay_ms(5); ser_putch(0); __delay_ms(5); ser_putch(200); __delay_ms(5); ser_putch(0); __delay_ms(5); ser_putch(1);__delay_ms(5);
}
void stop(void){
    ser_putch(DRIVE); __delay_ms(5); ser_putch(0); __delay_ms(5); ser_putch(0); __delay_ms(5); ser_putch(0); __delay_ms(5); ser_putch(0);__delay_ms(5);
}

void moveDistanceForward(int centimeters){
    RTC_MOVE_PATTERN_COUNTER = 0; //Reset the counter
    // 21053/4/100 = 52.6325 ---> milliseconds to move one centimeter
    float timeToMoveOneCentimeter = 104.7914/2; // Should probably be a float number instead
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
    int timeToMoveOneDegree = 11; // Should probably be a float number instead
    int totalTimeToTurn = degrees*timeToMoveOneDegree;
    //Set the time for the counter to wait until next step in pattern
    MOVE_PATTERN_TIME = totalTimeToTurn;
    turnCW();
    LED0 = 1;
    LED1 = 0;
    
}
void turnDegreesCCW(int degrees){
    RTC_MOVE_PATTERN_COUNTER = 0; //Reset the counter
    // 1130/90 = 12.55 ---> milliseconds to move one degree
    float timeToMoveOneDegree = 12; // Should probably be a float number instead
    int totalTimeToTurn = degrees*timeToMoveOneDegree;
    //Set the time for the counter to wait until next step in pattern
    MOVE_PATTERN_TIME = totalTimeToTurn;
    turnCCW();
    LED0 = 1;
    LED1 = 0;
    
}
//-----MOVE PATTERNS START-----

char followWallPattern(){
    if(patternStage == 0 && RTC_FLAG_MOVE_PATTERN){
        RTC_MOVE_PATTERN_COUNTER = 0; 
        MOVE_PATTERN_TIME = 300;
        int valueOff = latestReadMeterValue-30;
        valueOff*10; // Convert To millimeters
        int radius = 0;
        radius = 5000/valueOff;
        
        turnAndDrive(radius);
        //increment pattern stage
        patternStage++;
        //Reset Pattern Flag
        if(latestReadMeterValue != 0){
            RTC_FLAG_MOVE_PATTERN = 0;
        }
        
    }
    if(patternStage == 1 && RTC_FLAG_MOVE_PATTERN){
        LED0=1;
        //Move forward 400 cm
        moveDistanceForward(10);
        //increment pattern stage
        patternStage++;
        //Reset Pattern Flag
        patternStage = 0;
        RTC_FLAG_MOVE_PATTERN = 0;
    }
}

char moveTowardsWallPattern(int degree, int distance)
{
    if(patternStage == 0){
        LED0=1;
        //Move forward 400 cm
        turnDegreesCCW(degree);
        //increment pattern stage
        patternStage++;
        //Reset Pattern Flag
        RTC_FLAG_MOVE_PATTERN = 0;
    }
    if(patternStage == 1&&RTC_FLAG_MOVE_PATTERN){
        if(distance > 15){
            moveDistanceForward(distance);
            //Reset Pattern Flag
            RTC_FLAG_MOVE_PATTERN = 0;
        }
        patternStage++;
    }
    if(patternStage == 2&&RTC_FLAG_MOVE_PATTERN){
        //Move forward 400 cm
        turnDegreesCW(90);
        //increment pattern stage
        patternStage++;
        //Reset Pattern Flag
        RTC_FLAG_MOVE_PATTERN = 0;
    }
    else if (RTC_FLAG_MOVE_PATTERN&&patternStage == 3){
        //End of pattern stop the motor
        stop();
        //Reset pattern stage
        patternStage = 0;
        RTC_FLAG_MOVE_PATTERN = 0;
        
        LED0 = 0;
        LED1 = 0;
        // Return 1 to show that pattern is over
        return 1;
    }
    //Return 0 if pattern is not over
    return 0;
}

char moveStraightPattern()
{
    if(patternStage == 0){
        //Move forward 400 cm
        moveDistanceForward(400);
        //increment pattern stage
        patternStage++;
        //Reset Pattern Flag
        RTC_FLAG_MOVE_PATTERN = 0;
    }
    else if (RTC_FLAG_MOVE_PATTERN&&patternStage == 1){
        //End of pattern stop the motor
        stop();
        //Reset pattern stage
        patternStage = 0;
        RTC_FLAG_MOVE_PATTERN = 0;
        
        LED0 = 0;
        LED1 = 0;
        // Return 1 to show that pattern is over
        return 1;
    }
    //Return 0 if pattern is not over
    return 0;
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
    else if((RTC_FLAG_MOVE_PATTERN&&patternStage == 1)||(RTC_FLAG_MOVE_PATTERN&&patternStage == 3) || (RTC_FLAG_MOVE_PATTERN&&patternStage == 5)|| (RTC_FLAG_MOVE_PATTERN&&patternStage == 7)){
        //Move forward 100 cm
        turnDegreesCW(90);
        //increment pattern stage
        patternStage++;
        //Reset Pattern Flag
        RTC_FLAG_MOVE_PATTERN = 0;
    }
    else if (RTC_FLAG_MOVE_PATTERN&&patternStage == 8){
        //End of pattern stop the motor
        stop();
        //Reset pattern stage
        patternStage = 0;
        RTC_FLAG_MOVE_PATTERN = 0;
        
        LED0 = 0;
        LED1 = 0;
        // Return 1 to show that pattern is over
        return 1;
    }
    //Return 0 if pattern is not over
    return 0;
}

//-----MOVE PATTERNS END-----

//-----SENSOR READINGS START----
int getTraveledDistance(){
    int distance = 0;
    ser_putch(142); //Set to read sensors
    __delay_ms(5);
    ser_putch(19); //Set drive packet ID
    __delay_ms(5);
    distance = ser_getch()*256;
    __delay_ms(5);
    distance += ser_getch(); //Combine high bit with low bit 
    return distance;
}

void updateDistOnLCD(){
    if(updateLcdDistData){
        distanceTraveled += getTraveledDistance();
        lcdSetCursor(0x40);
        lcdWriteToDigitBCD(distanceTraveled, 4, 0);
        updateLcdDistData = 0;
    }
}


//-----SENSOR READINGS END-----