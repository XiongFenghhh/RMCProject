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
	 
	while(1){

		RC_Receive();
		RC_Convert();
	if(me.isStart)
			{
					RM_SystemSwitch(1);
			  i++; 

				pWord[1]=(unsigned char)(((int16_t)me.rotation_fil[0])>>8 & 0x00ff);
				pWord[0]=(unsigned char)(((int16_t)me.rotation_fil[0])& 0x00ff);
				pWord[3]=(unsigned char)((encoder_cnt[0])>>8 & 0x00ff);
				pWord[2]=(unsigned char)((encoder_cnt[0]) & 0x00ff);
				pWord[5]=(unsigned char)(((int16_t)me.errors[0])>>8 & 0x00ff);
				pWord[4]=(unsigned char)(((int16_t)me.errors[0])& 0x00ff);
				pWord[7]=(unsigned char)(((int16_t)me.pwm[0])>>8 & 0x00ff); 
				pWord[6]=(unsigned char)(((int16_t)me.pwm[0])& 0x00ff);
//				pWord[7]=(unsigned char)((encoder_cnt[2])>>8 & 0x00ff);
//				pWord[6]=(unsigned char)((encoder_cnt[2])& 0x00ff);
				//printf("Encoder:%d\r\n",encoder_cnt[3]);
				
				printf("error:%f\r\n",me.errors[3]);
				printf("pwm:%f\r\n",me.pwm[3]);
		   RS232_VisualScope( USART3, pWord, 8);
				
				
//		if(vscope_en)
//		{
//		   	vscope_en = 0;
//				RS232_VisualScope( USART3, pWord, 8);
//			}
			i%=30000;
			
			
			PIDAlgorithm();
			
			BMotor_PWM(1);
			BMotor_PWM(2);
			if(isAutoTarget()==1){
				setIsPitchTargeted(getYunTaiAdjustPitch()>=30?-1:getYunTaiAdjustPitch()<=-30?1:0);
				setIsYawTargeted(getYunTaiAdjustYaw()>=40?-1:getYunTaiAdjustYaw()<=-40?1:0);
					
			}

		if(getIsReceive()==1)
			{
				//outputData();	
				resetIsReceive();
				setMotoParameter();
			}

//		if(i==5000)printf("Encoder:%d\r\n",encoder_cnt[3]);		
//			if(i==25000)printf("ROTATION: %f \r\n",me.rotation[3]);
//			if(i==15000)printf("error: %f k: %f\r\n",me.errors[3],me.Kp[3]);

		}else RM_SystemSwitch(0);
	}

return 0;
	
}



