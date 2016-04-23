#include "common.h"
#include "iRobot.h"

void setupIRobot(void){
    ser_putch(START); //start create robot
    __delay_ms(5);
    ser_putch(FULL);
    __delay_ms(5);
}
void drive(void){
    ser_putch(DRIVE); __delay_ms(5); ser_putch(0); __delay_ms(5); ser_putch(200); __delay_ms(5); ser_putch(127); __delay_ms(5); ser_putch(255);__delay_ms(5);
}

void squareTest(void){
    ser_putch(152);__delay_ms(5);ser_putch(17);__delay_ms(5);ser_putch(137);__delay_ms(5);ser_putch(1);__delay_ms(5);ser_putch(44);__delay_ms(5);ser_putch(128);__delay_ms(5);ser_putch(0);__delay_ms(5);ser_putch(156);__delay_ms(5);ser_putch(1);__delay_ms(5);ser_putch(144);__delay_ms(5);ser_putch(137);__delay_ms(5);ser_putch(1);__delay_ms(5);ser_putch(44);__delay_ms(5);ser_putch(0);__delay_ms(5);ser_putch(1);__delay_ms(5);ser_putch(157);__delay_ms(5);ser_putch(0);__delay_ms(5);ser_putch(90);__delay_ms(5);ser_putch(153);__delay_ms(5);
}
void turnCW(){
    ser_putch(DRIVE); __delay_ms(5); ser_putch(0); __delay_ms(5); ser_putch(200); __delay_ms(5); ser_putch(255); __delay_ms(5); ser_putch(255);__delay_ms(5);
}
void stopAfter40(){
    ser_putch(152);__delay_ms(5);ser_putch(13);__delay_ms(5);ser_putch(137);__delay_ms(5);ser_putch(1);__delay_ms(5);ser_putch(44);__delay_ms(5);ser_putch(128);__delay_ms(5);ser_putch(0);__delay_ms(5);ser_putch(156);__delay_ms(5);ser_putch(1);__delay_ms(5);ser_putch(144);__delay_ms(5);ser_putch(137);__delay_ms(5);ser_putch(0);__delay_ms(5);ser_putch(0);__delay_ms(5);ser_putch(0);__delay_ms(5);ser_putch(0);__delay_ms(5);
}
void stop(void){
    ser_putch(DRIVE); __delay_ms(5); ser_putch(0); __delay_ms(5); ser_putch(0); __delay_ms(5); ser_putch(0); __delay_ms(5); ser_putch(0);__delay_ms(5);
}
