#include "main.h"

/**
*before the main() was excuted, the RCC and Flash are already configured in SystemInit();
*if you want to re-configure RCC and Flash goto SetSysClock();
*/
int main(){
int i=0;

	RM_Init();
	
	delay_ms(500);
//	 while(MPU6050_Initialization() == 0xff) 
//    {
//        i++;     //如果一次初始化没有成功，那就再来一次                     
//        if(i>10) //如果初始化一直不成功，那就没希望了，进入死循环，蜂鸣器一直叫
//        {
//            while(1) 
//            {
//								printf("MPU6050 Error\r\n");
//								delay_ms(50);
//                
//            }
//        }  
//    
//		}
//		
//printf("MPU6050 SUCCESS\r\n");
//	MPU6050_Gyro_calibration();
//    MPU6050_Interrupt_Configuration(); 
   RM_InterruptInit();
	TIM_Cmd(TIM1,ENABLE);
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	TIM_Cmd(TIM8,ENABLE);
	TIM_CtrlPWMOutputs(TIM8,ENABLE);
	batholithResetPwm();	
	while(1){
		i++;
		//RS232_VisualScope( USART3, pWord, 8);

		i%=10000;
		RC_Receive();
		RC_Convert();		
		if(me.isStart)
		{
			CalcRotations();
			PIDAlgorithm();
	  }
		else 
		batholithResetPwm();
		BMotor_PWM(1);
		BMotor_PWM(2);
		if(isAutoTarget()==1){
			setIsPitchTargeted(getYunTaiAdjustPitch()>=30?-1:getYunTaiAdjustPitch()<=-30?1:0);
			setIsYawTargeted(getYunTaiAdjustYaw()>=40?-1:getYunTaiAdjustYaw()<=-40?1:0);
				
		}
//if(me.isDebugging==1){
	if(getIsReceive()==1)
		{
			outputData();	
			resetIsReceive();
			setMotoParameter();
		}
		if(i==5000)printf("Encoder:%d %d %d %d\r\n",encoder_cnt[0],encoder_cnt[1],encoder_cnt[2],encoder_cnt[3]);		
		if(i==15000)printf("PWM:%f %f %f %f\r\n",me.pwm[0],me.pwm[1],me.pwm[2],me.pwm[3]);
		//	}
	}

return 0;
	
}



