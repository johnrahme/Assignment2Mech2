#include "common.h"
#include "iRobot.h"
#include "timer0.h"
#include "lcd.h"
#include "adConv.h"
#include "patternHandler.h"

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
//This function sets the wheel speeds independently
void turnAndDriveDirect(int rightVelocity, int leftVelocity){
    char highByteR = 0;
    char lowByteR = 0;
    
    char highByteL = 0;
    char lowByteL = 0;
    //Sets speed of Right Wheel
    if(rightVelocity <0){
        highByteR = -rightVelocity/256;
        lowByteR = -rightVelocity%256;
        highByteR = highByteR ^ 0b11111111;
        lowByteR = lowByteR ^ 0b11111111;
    }
    else{
        highByteR = rightVelocity/256;
        lowByteR = rightVelocity%256;
    }
    //Sets speed of left Wheel
    if(rightVelocity <0){
        highByteL = -leftVelocity/256;
        lowByteL = -leftVelocity%256;
        highByteL = highByteL ^ 0b11111111;
        lowByteL = lowByteL ^ 0b11111111;
    }
    else{
        highByteL = leftVelocity/256;
        lowByteL = leftVelocity%256;
    }
    ser_putch(DRIVE_DIRECT); __delay_ms(5); ser_putch(highByteR); __delay_ms(5); ser_putch(lowByteR); __delay_ms(5); ser_putch(highByteL); __delay_ms(5); ser_putch(lowByteL);__delay_ms(5);
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
}
void turnDegreesCW(int degrees){
    RTC_MOVE_PATTERN_COUNTER = 0; //Reset the counter
    // 1130/90 = 12.55 ---> milliseconds to move one degree
    int timeToMoveOneDegree = 11; // Should probably be a float number instead
    int totalTimeToTurn = degrees*timeToMoveOneDegree;
    //Set the time for the counter to wait until next step in pattern
    MOVE_PATTERN_TIME = totalTimeToTurn;
    turnCW();
    
}
void turnDegreesCCW(int degrees){
    RTC_MOVE_PATTERN_COUNTER = 0; //Reset the counter
    // 1130/90 = 12.55 ---> milliseconds to move one degree
    float timeToMoveOneDegree = 12; // Should probably be a float number instead
    int totalTimeToTurn = degrees*timeToMoveOneDegree;
    //Set the time for the counter to wait until next step in pattern
    MOVE_PATTERN_TIME = totalTimeToTurn;
    turnCCW();
    
}
//-----MOVE PATTERNS START-----

char followWallPattern(){
    if(patternStage == 0){
        RTC_MOVE_PATTERN_COUNTER = 0; 
        MOVE_PATTERN_TIME = 20;
        int valueOff = latestReadMeterValue-50;
        valueOff*10; // Convert To millimeters
        int radius = 0;
        radius = 20000/valueOff;
        
        turnAndDrive(radius);
        //increment pattern stage
        patternStage++;
        //Reset Pattern Flag
        RTC_FLAG_MOVE_PATTERN = 0;
    }
    else if(patternStage == 1 && RTC_FLAG_MOVE_PATTERN){
        RTC_MOVE_PATTERN_COUNTER = 0; 
        MOVE_PATTERN_TIME = 20;
        int valueOff = latestReadMeterValue-50;
        valueOff*10; // Convert To millimeters
        int radius = 0;
        radius = 20000/valueOff;
        
        turnAndDrive(radius);
        //Do not increment since we want this function to run forever
        //patternStage++;
        //Reset Pattern Flag
        RTC_FLAG_MOVE_PATTERN = 0;
    }
}
char moveTowardsWallPattern(int degree, int distance)
{
    if(patternStage == 0){
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
char getBumpDropSensor(){
    char bumpSensor = 0;
    ser_putch(142); //Set to read sensors
    __delay_ms(5);
    ser_putch(7); //Set drive packet ID
    __delay_ms(5);
    bumpSensor = ser_getch();
    return bumpSensor;
}

void updateBumpDropSensor(){
    char bumpSensor = getBumpDropSensor();
    char stopMovement = bumpSensor&0b00011111;
    if(stopMovement){
        stopAllPatterns();
    }
}
void updateSensors(){
    if(updateSensorsFlag){
        //Check bumpCliffSensors
        updateBumpDropSensor();
        //Update and write distance travelled
        distanceTraveled += getTraveledDistance();
        lcdSetCursor(0x40);
        lcdWriteToDigitBCD(distanceTraveled, 4, 0);
        updateSensorsFlag = 0;
    }
}


//-----SENSOR READINGS END-----