
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include "common.h"
int smallestDistance = 20000;
int scanStepNumber = 0;
int smallestValueStep = 0;
int scanRunning = 0;
int stepsToMove = 400;
int movingToWall = 0;
int stepsFromOrigin = 0;

char updateScanner();
void setScannerSpeed(char speed);
void resetToOrigin();

#endif	/* XC_HEADER_TEMPLATE_H */

