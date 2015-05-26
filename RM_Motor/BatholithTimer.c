#include "BatholithTimer.h"
static uint8_t deadTime;
static int piancha=55;
uint8_t getDeadTime(){return deadTime;}
void setDeadTime(uint8_t dt){deadTime=dt;}
/*
*Function: RMTIM_PWM_Config
*Description: 
*1.Config Timer1 and Timer 8 to output pwm wave
*2.use GPIOE8,9,10,11--------Timer 1;
   use Pc7,8  Pb0,1----------Timer 8;
*3.Timer8_ch2(pc7,pb0)------------Right Front
   Timer8_ch3(pc8,pb1)------------left Front
   Timer1_ch1(pe8,pe9)------------left Behind
   Timer1_ch2(pe10,pe11)-----------Right Behind
*Calls: RMTIM1_Config(),RMTIM8_Config()
*/
void RmBatholicTIM_PWM_Config(){
RmBatholicTIM1_Config();
RmBatholicTIM8_Config();
}
void RmBatholicTIM1_Config(){
 	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_BDTRInitTypeDef bdStructure;
	uint16_t PrescalerValue = (uint16_t) (SystemCoreClock / 10000000) - 1;
	
	/* TIM8 clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  /* GPIOD clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	
	
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
	    
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_TIM1);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1); 

	RCC_TIMCLKPresConfig(RCC_TIMPrescActivated);
		
 
  /* Compute the prescaler value */

	TIM_DeInit(TIM1);
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 999;
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 300;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCIdleState_Reset;
	
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	
	bdStructure.TIM_OSSRState=TIM_OSSRState_Enable;
	bdStructure.TIM_OSSIState=TIM_OSSIState_Enable;
	bdStructure.TIM_LOCKLevel=TIM_LOCKLevel_OFF;
	bdStructure.TIM_DeadTime=0xD0;
	bdStructure.TIM_Break=TIM_Break_Disable;
	bdStructure.TIM_BreakPolarity=TIM_BreakPolarity_High;
  bdStructure.TIM_AutomaticOutput=TIM_AutomaticOutput_Enable;
	TIM_BDTRConfig(TIM1,&bdStructure);
  	
	TIM_ARRPreloadConfig(TIM1, ENABLE);
  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
	
	
}
void RmBatholicTIM8_Config(){
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_BDTRInitTypeDef bdStructure;
  /* Compute the prescaler value */
	uint16_t PrescalerValue = (uint16_t) (SystemCoreClock / 10000000) - 1;
	GPIO_InitTypeDef GPIO_InitStructure;
	/* TIM4 clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);

  /* GPIOD clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
  

	
	 GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7 | GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	

	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOB, &GPIO_InitStructure); 

    
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM8);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM8);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM8);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM8); 
  
  

	RCC_TIMCLKPresConfig(RCC_TIMPrescActivated);
	
		
	
	
 

	TIM_DeInit(TIM8);
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 999;
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);
	

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 500;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCIdleState_Reset;
	
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);
	TIM_OC3Init(TIM8, &TIM_OCInitStructure);
	


	 

	bdStructure.TIM_OSSRState=TIM_OSSRState_Enable;
	bdStructure.TIM_OSSIState=TIM_OSSIState_Enable;
	bdStructure.TIM_LOCKLevel=TIM_LOCKLevel_OFF;
	bdStructure.TIM_DeadTime=0xD0;
	bdStructure.TIM_Break=TIM_Break_Disable;
	bdStructure.TIM_BreakPolarity=TIM_BreakPolarity_High;
  bdStructure.TIM_AutomaticOutput=TIM_AutomaticOutput_Enable;
	TIM_BDTRConfig(TIM8,&bdStructure);
  
	
	TIM_ARRPreloadConfig(TIM8, ENABLE);
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);

	
	
}
/*
*@date 2015.4.14
*add protection for pwm because PID parameter isn't perfect yet
*/
void batholithSetPwm(){
	if(me.isPWMallowed==0)
	{
			TIM_Cmd(TIM8,ENABLE);
			TIM_CtrlPWMOutputs(TIM8,ENABLE);
			TIM_Cmd(TIM1,ENABLE);
			TIM_CtrlPWMOutputs(TIM1,ENABLE);
			me.isPWMallowed=1;
	}
	TIM_SetCompare2(TIM8,me.pwm[0]);
	TIM_SetCompare3(TIM8,me.pwm[1]);
	TIM_SetCompare1(TIM1,me.pwm[2]);
	TIM_SetCompare2(TIM1,me.pwm[3]-piancha);

	
}
void batholithResetPwm(){
	
	TIM_Cmd(TIM8,DISABLE);
	TIM_CtrlPWMOutputs(TIM8,DISABLE);
	TIM_Cmd(TIM1,DISABLE);
	TIM_CtrlPWMOutputs(TIM1,DISABLE);
	me.isPWMallowed=0;
}

