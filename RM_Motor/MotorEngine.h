/**************************************
 File name: VelocityRotationPWM.h
 Author:ÐÜ·å Date:2015-5-1
 Description: MotorEngine contains related information about motors
**************************************/
#ifndef VelocityRotationPWM_h
#define VelocityRotationPWM_h

//#include "Velocity.h"


#include "RMVariables.h"



#define abs(x) ((x)>0? (x):(-(x)))

struct MotorEngine{
	uint8_t isDebugging;
	
	double rotation[4];
	double rotation_fil[4];
	
	double Kp[4];
	double Ki[4];
	double Kd[4];
	double deltaT;
	double errors[4][2];
	double maxAbs;
	

  uint32_t pwm[4];
	int isClock[4];
	
	int isStart;
	uint8_t isPWMallowed;
};


void MotorEngine_Config(void);
void CalcRotations(void);

void PIDAlgorithm(void);
void PIDAlgorithm1(void);
void PIDAlgorithm2(void);
void PIDAlgorithm3(void);
void PIDAlgorithm4(void);

#endif
