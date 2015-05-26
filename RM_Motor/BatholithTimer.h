#ifndef BatholithTimer_h
#define BatholithTimer_h
#include "RMVariables.h"
void RmBatholicTIM_PWM_Config(void);
void RmBatholicTIM8_Config(void);
void RmBatholicTIM1_Config(void);
void batholithSetPwm(void);
void batholithResetPwm(void);
void setDeadTime(uint8_t);
uint8_t getDeadTime();
#endif
