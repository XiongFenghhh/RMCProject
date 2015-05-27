/**
  ******************************************************************************
  * @file    LTDC_Display_2Layers/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"

/** @addtogroup STM32F429I_DISCOVERY_Examples
  * @{
  */

/** @addtogroup LTDC_Display_2Layers
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t itTimes=0;
static uint8_t shootCount=0;
static uint8_t brushStart=0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
//@TODO Feedback from Encoder
void SysTick_Handler(void)
{
	itTimes++;
	itTimes=itTimes%8;
	Encoder_Get();

if(itTimes%2==0){	
	if(setXSpeed-40>realXSpeed)
	{
		if(setXSpeed==0)realXSpeed+=35;
		else realXSpeed+=35;
	}
	else if(setXSpeed+40<realXSpeed)
	{
		if(setXSpeed==0)realXSpeed-=35;
		else realXSpeed-=35;
	}
		
		
	else realXSpeed=setXSpeed;
	/*we might use the first dimension of errors to implement differential controll*/
	me.errors[0][0]=me.errors[0][1];
	me.errors[1][0]=me.errors[1][1];
	me.errors[2][0]=me.errors[2][1];
	me.errors[3][0]=me.errors[3][1];
	
	me.errors[0][1]=me.rotation_fil[0];//-encoder_cnt[0]/1024*60;
	me.errors[1][1]=me.rotation_fil[1];//-encoder_cnt[1]/1024*60;
	me.errors[2][1]=me.rotation_fil[2];//-encoder_cnt[2]/1024*60;
	me.errors[3][1]=me.rotation_fil[3];//-encoder_cnt[3]/1024*60;
	
}else{
if(setYSpeed-40>realYSpeed)
{
	if(setYSpeed==0)realYSpeed+=35;
	else realYSpeed+=35;
}else if(setYSpeed+20<realYSpeed)
	{
		if(setYSpeed==0)realYSpeed-=15;
		else realYSpeed-=35;
	}
	else realYSpeed=setYSpeed;

}
	
	/**
	*shooting protection
	*/
	if(getBMPWM()>200)
		{
			if(brushStart<15)
			{
				brushStart++;
				GPIO_ResetBits(GPIOD,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);
			}
			if(isStepperMoving()==1&&brushStart>10){
			Stepper_Ctrl(itTimes);
		}
		}
	else 
	{
			brushStart=0;
			GPIO_ResetBits(GPIOD,GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);
	}
	/****************************************/
		
 
}


/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f429_439xx.s).  */
/******************************************************************************/

/******************************************************************************
* @fn DMA1_Stream5_IRQHandler
*
* @brief USART2 DMA ISR
*
* @return None.
*
* @note This code is fully tested on STM32F405RGT6 Platform, You can port it
* to the other platform.
*/

/**
  * @}
  */ 

/**
  * @}
  */ 
	
void DMA1_Stream5_IRQHandler(void)
{
		if(DMA_GetITStatus(DMA1_Stream5, DMA_IT_TCIF5))
		{
				DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TCIF5);
			
				
			
		}
}
void TIM3_IRQHandler(void)
{  
	

}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
