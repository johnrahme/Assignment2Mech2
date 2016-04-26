/*********************************************************************************
	Description: Controls stepper motor from debounced buttons      			 
	Uses: DSX Experimenter board and SMPM																			 
	Connections:
				 Port C - stepper motor via the SMPM                																 
																				 
	Author: Dinuka Abeywardena/Kyle Alvarez   
	Note: Do note that this is only a test file and is not a proper header/source file pair
	Feel free to use this as a starting point for SPI communication
	Although I expect when you use it in your project, you will appropriately convert this into its respective source/header pair             																 
																				 
**********************************************************************************/
#include <xc.h>
#include "SPI.h"

// Interrupt service routine
// Interrupt service routine


void setupSPI(void){
    //PortB
    TRISB =  0b00000000;

    SSPSTAT = 0b01000000;
    SSPCON = 0b00100000;
    TRISC = 0b11010000;
    PORTC = 0b00000011;

}

//Transfers data through SPI using SSPBUF/SM_Control_Byte. Note used interchangeably here for illustration purposes
unsigned char spi_transfer(unsigned char data){
    unsigned char temp = 0;

    SSPIF = 0;
    SSPBUF = data;

    while (SSPIF == 0);
    temp = SSPBUF;
    SSPIF = 0;

    return temp;
}

void setToMotorCW(){
    unsigned char controlByte = 0b00001101;
    spi_transfer(controlByte);
}
