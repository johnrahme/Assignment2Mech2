#include "ser.h"
#include "irobot.h"

void irobot_init(void){
	ser_putch(128);	//setup iRobot START OPCODE
	ser_putch(132);	//activate FULL MODE
}

void drive(void){
    ser_putch(137);ser_putch(0);ser_putch(200);ser_putch(127);ser_putch(225);
}

void stop(void){
    
}

signed int get_distance(void){								
}

void drive_distance(signed int requiredDistance){
}


void store_song(void){
}

void play_song(void){
}

unsigned char get_cliff(void){
}

void drive_square(void){
}

void drive_angle(signed int requiredAngle, unsigned char direction){
}

signed int get_angle(void){								
}