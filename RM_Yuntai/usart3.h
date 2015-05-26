#ifndef __USART1_H__
#define __USART1_H__

#include "stm32f4xx.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "RMVariables.h"

void USART3_Configuration(void);
void USART3_SendChar(unsigned char b);
void USART3_IRQHandler(void);
//called by main()
int16_t getYunTaiAdjustYaw(void);
int16_t getYunTaiAdjustPitch(void);
void setMotoParameter(void);
void resetIsReceive(void);
uint8_t getIsReceive(void);
#endif
