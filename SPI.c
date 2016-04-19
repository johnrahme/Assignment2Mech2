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
#pragma config BOREN = OFF, CPD = OFF, WRT = OFF, FOSC = HS, WDTE = OFF, CP = OFF, LVP = OFF, PWRTE = OFF

//Defines
//Osillator (Hz)
#define _XTAL_FREQ 20000000

#define TMR0_VAL 100	//timer0 start count
#define STEPS180  200	//half steps for a 180 degree rotation
#define SM_Control_Byte SSPBUF	//SM_Control_Byte is SSPBUF

//Macros that you can call for ease of use
#define     SELECT_NONE()	RC0 = 0; RC1 = 0;
#define     SELECT_EEPROM()	RC0 = 0; RC1 = 1;
#define     SELECT_RF()		RC0 = 1; RC1 = 0;
#define     SELECT_SM()		RC0 = 1; RC1 = 1;
#define     SM_STEP()		RC2 = 1; NOP(); RC2 = 0;


volatile unsigned int time_count;
volatile bit FLAG_1000MS;

//Function prototypes
void setupSPI(void);
unsigned char spi_transfer(unsigned char data);

// Interrupt service routine
void interrupt isr(void){
//Timer 1
    if(TMR0IF){
        TMR0IF = 0;
        TMR0 = TMR0_VAL;
        time_count++;

        if(time_count % 1 == 0) {
            //FLAG_10MS = 1;
            SM_STEP();
        }

        if(time_count % 1000 == 0){
            FLAG_1000MS = 1;	// Raise flag for 500ms
            time_count = 0;	// Clear time_count
        }
    }
}

void setupSPI(void){
    //PortB
    TRISB =  0b00000000;

    SSPSTAT = 0b01000000;
    SSPCON = 0b00100000;
    TRISC = 0b11010000;
    PORTC = 0b00000011;

    //timer0
    OPTION_REG = 0b00000100;
    //enable timer0 interrupt
    TMR0IE = 1;

    //Enable all interrupts
    ei();
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

void main(void){
    setupSPI();
    unsigned char controlByte = 0b00001101;
    spi_transfer(controlByte);
    while(1){
        if(FLAG_1000MS){
            RB0 = !RB0;
            FLAG_1000MS = 0;
        }
    }
}