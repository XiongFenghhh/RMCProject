#include "RMVariables.h"
struct MotorEngine me;
struct RC_Ctl_t RC_Ctl;
MPU6050_RAW_DATA    MPU6050_Raw_Data; 
MPU6050_REAL_DATA   MPU6050_Real_Data;
int16_t encoder_cnt[4]={0};

double setXSpeed;
double realXSpeed;
double setYSpeed;
double realYSpeed;

double realWSpeed;
double preX;
double preY;
uint8_t yunTaiProtectionMode;
double Kp=8;
double Ki=0.5;
double Kd=0.15;

unsigned char pWord[10];
unsigned char vscope_en = 1;


void delay_ms(uint16_t t){
int i;
	for( i=0;i<t;i++)
	{
		int a=42000; //at 168MHz 42000 is ok
		while(a--);
	}
}

void RM_Init(void){
	MotorEngine_Config();
	RC_Init();
	RmBatholicTIM_PWM_Config();
	Stepper_Init();
	BMotor_Init();
	Encoder_Init();
	CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_4tq,CAN_BS1_9tq,3,CAN_Mode_Normal);
	USART3_Configuration();
	
	
	//following functions is mainly for debugging purpose
	
}
void RM_InterruptInit(void){

		if (SysTick_Config(SystemCoreClock / 200))
  { 
    /* Capture error */ 
    while (1);
  }
}
/**
*@function RM_SystemSwitch()
*@description Turn on or shut down the system
*@para mode:If mode is 1 turn on the system. If mode is 0 shut down the system.
*@retVal none
*/
void RM_SystemSwitch(uint8_t mode){
if(mode==1&&me.isRun==0)
	{
		MotorEngineReset();
		batholithResetPwm();
		batholithPWMEnable();
		brushLessEnable();
		Encoder_Start();
		RC_Reset();
		setXSpeed=0;
		setYSpeed=0;
		realWSpeed=0;
		yunTaiProtectionMode=0;
		CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
		GPIO_ResetBits(GPIOC,GPIO_Pin_10);
		me.isRun=1;
	}
if(mode==0&&me.isRun==1)
	{
		MotorEngineReset();
		batholithResetPwm();
		batholithPWMDisable();
		brushLessDisable();
		Encoder_Disable();
		RC_Reset();
		setXSpeed=0;
		setYSpeed=0;
		realWSpeed=0;
		yunTaiProtectionMode=1;
	  GPIO_SetBits(GPIOC,GPIO_Pin_10);
		me.isRun=0;
	}
}
