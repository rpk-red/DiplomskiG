#ifndef _uartG_H
#define _uartG_H


#include "stm32f4xx.h"



#define MAX_BUFFER_SIZE 100


extern volatile char InputBufferUSART2[MAX_BUFFER_SIZE];


void InitUART();


void SendChar(unsigned char c);
void SendString(unsigned char *str);
//void SendEcho();
void AnalyzeRecivedData();

void ClearUSART2Buffer(void);



#endif
