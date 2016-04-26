#ifndef SPI_H
#define	SPI_H

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
void setToMotorCW(void);



#endif	/* SPI_H */

