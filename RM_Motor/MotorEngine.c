#include "MotorEngine.h"
#define set_feedback_sameDirection 1
#define set_feedback_diffDirection 2
#define set_feedback_toZero				 3
#define set_feedback_fromZero				4
static double xRatio;
static double yRatio;
static double wRatio;
static double preR1;
static double preR2;
static double preR3;
static double preR4;
static double inte = 0;
#define pwmMax 4800
#define pwmMin 0
#define pwmMiddle 5000
#define cumulationLimit 2000
uint8_t judgeState(uint8_t ch)
{
	if(me.rotation_fil[ch]*encoder_cnt[ch]>0&&abs(me.rotation_fil[ch])>=30&&abs(encoder_cnt[ch])>=30)return set_feedback_sameDirection;
	else if(me.rotation_fil[ch]*encoder_cnt[ch]<0&&abs(me.rotation_fil[ch])>=30&&abs(encoder_cnt[ch])>=30) return set_feedback_diffDirection;
	else if(abs(me.rotation_fil[ch])==0&&encoder_cnt[ch]!=0)return set_feedback_toZero;
	else if(me.rotation_fil[ch]!=0&&abs(encoder_cnt[ch])<30) return set_feedback_fromZero;
	else return 0;
}

/**
*@function PIDAlgorithmAll
*@discription excute PID Algorithm for 4 wheels
*@para channel: 0-----the first wheel.
								1-----the second wheel.
								2-----the third wheel.
								3-----the fourth wheel.
*@retVal none
*/
double Velocity_Control(double error,double rotation)
{
   
    static double error_v[2] = {0.0,0.0};
    static double output = 0;
    
		
    error_v[0] = error_v[1];
    error_v[1] = error;
    if(abs(rotation)<5)
		{
		inte += error;
		if(inte>cumulationLimit)inte=cumulationLimit;
		
		else if(inte<-cumulationLimit)inte=-cumulationLimit;
		}
		#ifndef Gun
		output=error_v[1] * Kp           
             + (error_v[1] - error_v[0]) * Kd;
		#endif
		#ifdef Gun

				output=error_v[1] * Kp           
             + (error_v[1] - error_v[0]) * Kd;
		#endif
     
    return output;//cyq:for6015 反向
}

void PIDAlgorithmAll(uint8_t ch)
{

	int channel=ch-1;
//	double pwm;
//	me.pwm[channel]= Velocity_Control(me.errors[channel]);

	me.pwm[channel]=Velocity_Control(me.errors[channel],me.rotation_fil[channel]);
	#ifdef Gun
	if(channel==0&&me.rotation_fil[channel]==0)me.pwm[channel]-=515;
	if(channel==1&&me.rotation_fil[channel]==0)me.pwm[channel]+=200;
	if(channel==2&&me.rotation_fil[channel]==0)me.pwm[channel]+=350;
	#endif
	if(me.pwm[channel]<-pwmMax)me.pwm[channel]=-pwmMax;
	else if(me.pwm[channel]>pwmMax)me.pwm[channel]=pwmMax;
	
}
/**
*@discription reset all content of Motorengine
*@para none
*@retVal none
*/
void MotorEngineReset(void)
{
	me.Kd[0]=Kd;
	me.Ki[0]=Ki;
	me.Kp[0]=Kp;
	
	me.Kd[1]=Kd;
	me.Ki[1]=Ki;
	me.Kp[1]=Kp;
	
	me.Kd[2]=Kd;
	me.Ki[2]=Ki;
	me.Kp[2]=Kp;
	
	me.Kd[3]=Kd;
	me.Ki[3]=Ki;
	me.Kp[3]=Kp;
	
	me.pwm[0]=0;
	me.pwm[1]=0;
	me.pwm[2]=0;
	me.pwm[3]=0;
	
	me.isPIDAllowed=0;
	me.rotation[0]=me.rotation_fil[0]=0;
	me.rotation[1]=me.rotation_fil[1]=0;
	me.rotation[2]=me.rotation_fil[2]=0;
	me.rotation[3]=me.rotation_fil[3]=0;
}
void MotorEngine_Config(void){
	
	 MotorEngineReset();
	
	me.isStart=0;//main while running flag
	me.isPWMallowed=0;//pwm generation flag
	me.isRun=0;//re-initialization
	
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
	preR1=me.rotation[0];
	//n1=-vx/(pi*D*tan(alpha))-vy/(pi*D)+vw*（lf-lw*tan(alpha))/(2*pi*D*tan(alpha))
	//No3:+++//No4-++
me.rotation[0]= -getXvelocity() / xRatio+getYvelocity() /yRatio-getWvelocity() *wRatio ;
me.rotation[0] = ( 300 * me.rotation[0] ) ;
me.rotation_fil[0]=0.1*preR1+me.rotation[0]*0.9;


	preR2=me.rotation[1];
	//n2=vx/(pi*D*tan(alpha))+vy/(pi*D)+（-lf-lw*tan(alpha))/(2*pi*D*tan(alpha))
	//No3:+-+//No4:--+
me.rotation[1]= getXvelocity() / xRatio+getYvelocity() / yRatio+getWvelocity() * wRatio;
me.rotation[1]=(300*me.rotation[1]);
me.rotation_fil[1]=0.1*preR2+me.rotation[1]*0.9;
	
	
	preR3=me.rotation[2];
	//n3=vx/(pi*D*tan(alpha))+vy/(pi*D)+（lf-lw*tan(alpha))/(2*pi*D*tan(alpha)) /red:-,+,-
	//No3:--+//No4:+-+
me.rotation[2]=-getXvelocity() /xRatio+getYvelocity() /yRatio+getWvelocity() *wRatio;
me.rotation[2]=(300*me.rotation[2]);
me.rotation_fil[2]=0.1*preR3+me.rotation[2]*0.9;

	preR4=me.rotation[3];
	//n4=vx/(pi*D*tan(alpha))-vy/(pi*D)+（lf+lw*tan(alpha))/(2*pi*D*tan(alpha))
	//No3:-++//No4:+++
me.rotation[3]=getXvelocity() / xRatio+getYvelocity() / yRatio-getWvelocity() * wRatio;
me.rotation[3]=(300*me.rotation[3]);
me.rotation_fil[3]=0.1*preR4+me.rotation[3]*0.9;

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

void PIDAlgorithm(void)
{
	if(me.isPIDAllowed){
		CalcRotations();
		PIDAlgorithmAll(1);
		PIDAlgorithmAll(2);
		PIDAlgorithmAll(3);
		PIDAlgorithmAll(4);
		batholithSetPwm();
		me.isPIDAllowed=0;
	}
}







