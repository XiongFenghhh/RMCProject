#include "RMVariables.h"
struct MotorEngine me;
struct RC_Ctl_t RC_Ctl;
MPU6050_RAW_DATA    MPU6050_Raw_Data; 
MPU6050_REAL_DATA   MPU6050_Real_Data;
int16_t encoder_cnt[4]={0};
double fbSpeedRatio=0;
double lrSpeedRatio=0;
double setXSpeed;
double realXSpeed;
double setYSpeed;
double realYSpeed;
unsigned char pWord[10];
unsigned char vscope_en = 0;

void delay_ms(uint16_t t){
int i;
	for( i=0;i<t;i++)
	{
		int a=42000; //at 168MHz 42000 is ok
		while(a--);
	}
}

void RM_Init(void){
	RmBatholicTIM_PWM_Config();
	MotorEngine_Config();
	RC_Init();
	Stepper_Init();
	BMotor_Init();
	CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS2_4tq,CAN_BS1_9tq,3,CAN_Mode_Normal);
	USART3_Configuration();
	Encoder_Init();
	
	//following functions is mainly for debugging purpose
	
}
void RM_InterruptInit(void){

CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
		if (SysTick_Config(SystemCoreClock / 200))
  { 
    /* Capture error */ 
    while (1);
  }
}