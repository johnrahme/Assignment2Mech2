
#ifndef ADCONV_H
#define ADCONV_H
#include <xc.h>
#include "common.h"
volatile bit conversionDone = 0; // Bit if conversion is done
void initializeADC();
int readADCData();
int readADCMeter();
void startADCConversion();
void printADCData();
int latestReadMeterValue = 0;


#endif ADCONV_H