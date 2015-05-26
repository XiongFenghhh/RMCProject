#include "MotorEngine.h"

static double xRatio;
static double yRatio;
static double wRatio;
static double preR1[5];
static double preR2[5];
static double preR3[5];
static double preR4[5];
void MotorEngine_Config(void){
me.deltaT=0.01;
	
	me.Kd[0]=0;
	me.Ki[0]=0;
	me.Kp[0]=1;
	
	me.Kd[1]=0;
	me.Ki[1]=0;
	me.Kp[1]=1;
	
	me.Kd[2]=0;
	me.Ki[2]=0;
	me.Kp[2]=1;
	
	me.Kd[3]=0;
	me.Ki[3]=0;
	me.Kp[3]=1;
	
	me.pwm[0]=500;
	me.pwm[1]=500;
	me.pwm[2]=500;
	me.pwm[3]=500;
	
	me.isStart=0;
	me.isPWMallowed=0;
	me.isDebugging=0;
	xRatio=PI * tan(McnalmAngle) * McnalmDiameter;
	yRatio= PI * McnalmDiameter;
	wRatio=( dipanHengZhouju * tan(McnalmAngle) + dipanZongZhouju ) / 2 / PI / McnalmDiameter / tan(McnalmAngle);
}
/********************************************

//@TODO

Function: CalcRotations
Description:Calculate rotations of each wheel,based on the velocity
						of the vheichle given by remoter.

Calls:  
Called by：main
Input: 	void
Output： rotations of 4 wheels.
return： 无
********************************************/
void CalcRotations(void)
{
	preR1[0]=preR1[1];
	preR1[1]=preR1[2];
	preR1[2]=preR1[3];
	preR1[3]=preR1[4];
	preR1[4]=me.rotation[0];
	//n1=-vx/(pi*D*tan(alpha))-vy/(pi*D)+vw*（lf-lw*tan(alpha))/(2*pi*D*tan(alpha))
	/*pwm3 on board blue:-,-,-*/	/*red:+,+,+*/
me.rotation[0]= getXvelocity() / xRatio+getYvelocity() /yRatio+getWvelocity() *wRatio ;
me.rotation[0] = ( 60 * me.rotation[0] ) ;
me.rotation_fil[0]=0.1*(preR1[0]+preR1[2]+preR1[3]+preR1[4]+preR1[1])+me.rotation[0]*0.5;

	preR2[0]=preR2[1];
	preR2[1]=preR2[2];
	preR2[2]=preR2[3];
	preR2[3]=preR2[4];
	preR2[4]=me.rotation[1];
	//n2=vx/(pi*D*tan(alpha))+vy/(pi*D)+（-lf-lw*tan(alpha))/(2*pi*D*tan(alpha))blue:+,-,+ /red+,-, +
me.rotation[1]= getXvelocity() / xRatio-getYvelocity() / yRatio+getWvelocity() * wRatio;
me.rotation[1]=(60*me.rotation[1]);
me.rotation_fil[1]=0.1*(preR2[0]+preR2[2]+preR2[3]+preR2[4]+preR2[1])+me.rotation[1]*0.5;
	
	
	preR3[0]=preR3[1];
	preR3[1]=preR3[2];
	preR3[2]=preR3[3];
	preR3[3]=preR3[4];
	preR3[4]=me.rotation[2];
	//n3=vx/(pi*D*tan(alpha))+vy/(pi*D)+（lf-lw*tan(alpha))/(2*pi*D*tan(alpha)) /red:-,+,-
	//4.14 change the sign of y for left behind blue:-,+,-
me.rotation[2]=-getXvelocity() /xRatio-getYvelocity() /yRatio+getWvelocity() *wRatio;
me.rotation[2]=(60*me.rotation[2]);
me.rotation_fil[2]=0.1*(preR3[0]+preR3[2]+preR3[3]+preR3[4]+preR3[1])+me.rotation[2]*0.5;

	preR4[0]=preR4[1];
	preR4[1]=preR4[2];
	preR4[2]=preR4[3];
	preR4[3]=preR4[4];
	preR4[4]=me.rotation[3];
	//n4=vx/(pi*D*tan(alpha))-vy/(pi*D)+（lf+lw*tan(alpha))/(2*pi*D*tan(alpha))blue:-,-,- red:+++
me.rotation[3]=-getXvelocity() / xRatio+getYvelocity() / yRatio+getWvelocity() * wRatio;
me.rotation[3]=(60*me.rotation[3]);
me.rotation_fil[3]=0.1*(preR4[0]+preR4[1]+preR4[2]+preR4[3]+preR4[4])+me.rotation[3]*0.5;

}


/****************************************
Function: PIDAlgorithm
Description:输入error进行PID控制，输出PWM占空比
Calls: PIDAlgorithm1~4
Called by：主函数
Input： InputReference，InputFeedback
Output： OutputControl，error
return：无
****************************************/
#define pwmMax 980
#define pwmMin 20
#define pwmMiddle 250
void PIDAlgorithm(void)
{
double maxAbs;
maxAbs=abs(me.errors[0][1]);
if(abs(me.errors[1][1])>maxAbs)maxAbs=abs(me.errors[1][1]);
if(abs(me.errors[2][1])>maxAbs)maxAbs=abs(me.errors[2][1]);
if(abs(me.errors[3][1])>maxAbs)maxAbs=abs(me.errors[3][1]);
	if(maxAbs>250)
	{
		me.errors[0][1]/=(maxAbs/250);
		me.errors[1][1]/=(maxAbs/250);
		me.errors[2][1]/=(maxAbs/250);
		me.errors[3][1]/=(maxAbs/250);
	}else if(maxAbs<18)
	{
		batholithResetPwm();
	}else
	{
		PIDAlgorithm1();
		PIDAlgorithm2();
		PIDAlgorithm3();
		PIDAlgorithm4();	
		batholithSetPwm();
	}
}


void PIDAlgorithm1(void)
{
	
	me.Kp[0]=0.9;

	me.pwm[0]=(me.errors[0][1]+pwmMiddle)<pwmMin?pwmMin:(me.Kp[0]*(me.errors[0][1])+pwmMiddle)/500*1000;
	me.pwm[0]=me.pwm[0]>pwmMax?pwmMax:me.pwm[0];
 
}

void PIDAlgorithm2(void)
{
	me.Kp[1]=0.9;

	me.pwm[1]=(me.errors[1][1]+pwmMiddle)<pwmMin?pwmMin:(me.Kp[1]*(me.errors[1][1])+pwmMiddle)/500*1000;
	me.pwm[1]=me.pwm[1]>pwmMax?pwmMax:me.pwm[1];
 
}

void PIDAlgorithm3(void)
{

	me.Kp[2]=0.9;

	me.pwm[2]=(me.errors[2][1]+pwmMiddle)<pwmMin?pwmMin:(me.Kp[2]*(me.errors[2][1])+pwmMiddle)/500*1000;
	me.pwm[2]=me.pwm[2]>pwmMax?pwmMax:me.pwm[2]; 
}

void PIDAlgorithm4(void)
{

	me.Kp[3]=0.9;

	me.pwm[3]=(me.errors[3][1]+pwmMiddle)<pwmMin?pwmMin:(me.Kp[3]*(me.errors[3][1])+pwmMiddle)/500*1000;
	me.pwm[3]=me.pwm[3]>pwmMax?pwmMax:me.pwm[3];
}





