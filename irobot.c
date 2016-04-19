#include "irobot.h"

void irobot_init(void){
	putch(128);	//setup iRobot START OPCODE
	putch(132);	//activate FULL MODE
}

void drive(void){
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