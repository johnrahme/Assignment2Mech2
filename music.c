/*
 * File:   music.c
 * Author: JohnMac
 *
 * Created on den 3 maj 2016, 16:28
 */


#include "music.h"
void setupMarioTheme(){
    ser_putch(140); __delay_ms(5); ser_putch(1); __delay_ms(5);ser_putch(7); __delay_ms(5);//Set song nr 1, Song length 7
    ser_putch(76); __delay_ms(5); ser_putch(10); __delay_ms(5);//E5 for 8/64 second
    ser_putch(76); __delay_ms(5); ser_putch(20); __delay_ms(5);//E5 for 16/64 second
    ser_putch(76); __delay_ms(5); ser_putch(20); __delay_ms(5);//E5 for 16/64 second
    
    ser_putch(72); __delay_ms(5); ser_putch(10); __delay_ms(5);//C5 for 8/64 second
    ser_putch(76); __delay_ms(5); ser_putch(20); __delay_ms(5);//E5 for 16/64 second
    ser_putch(79); __delay_ms(5); ser_putch(40); __delay_ms(5);//G5 for 32/64 second
    
    ser_putch(67); __delay_ms(5); ser_putch(20); __delay_ms(5);//G4 for 32/64 second    
}

void setupSongs(){
    setupMarioTheme();
    
}

void playSong(){
    ser_putch(141); __delay_ms(5); ser_putch(1);
}