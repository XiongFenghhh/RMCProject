#include "main.h"
//#define _yawtest
/**
*before the main() was excuted, the RCC and Flash are already configured in SystemInit();
*if you want to re-configure RCC and Flash goto SetSysClock();
*/
int main(){
int i=0;

	RM_Init();
	delay_ms(500);
	follow_yaw_en = 1;
//	 while(MPU6050_Initialization() == 0xff && i<10) 
//    {
//        i++;     //如果一次初始化没有成功，那就再来一次                     
//        if(i == 10) //如果初始化一直不成功，那就没希望了，进入死循环，蜂鸣器一直叫
//        {
//								printf("MPU6050 Error\r\n");
//								delay_ms(50);
//					      follow_yaw_en = 0;
//        }	
//		}
//		if(i < 10) printf("MPU6050 SUCCESS\r\n");
//		
//  
//	MPU6050_Gyro_calibration(); //获取6轴的偏移量，用作零点矫正，记得开启
//// 因为陀螺要积分求角度，readmpu6050放在定时中断中
/////    MPU6050_Interrupt_Configuration();//应该不需要中断读MPU6050

   RM_InterruptInit();
	 
	while(1){

		RC_Receive();
		RC_Convert();
//		#ifdef _yawtest
//		pWord[1]=(unsigned char)(((int16_t)MPU6050_Real_Data.Gyro_Z)>>8 & 0x00ff);
//				pWord[0]=(unsigned char)(((int16_t)MPU6050_Real_Data.Gyro_Z) & 0x00ff);
//				pWord[3]=(unsigned char)((500)>>8 & 0x00ff);
//				pWord[2]=(unsigned char)((500) & 0x00ff);
//				pWord[5]=(unsigned char)((int16_t)yaw & 0x00ff);
//				pWord[4]=(unsigned char)((int16_t)yaw & 0x00ff);
//		   RS232_VisualScope( USART3, pWord, 8);
//		#endif
	if(me.isStart==1)
			{
					RM_SystemSwitch(1);
			  i++;  
				pWord[1]=(unsigned char)(((int16_t)me.rotation_fil[0])>>8 & 0x00ff);
				pWord[0]=(unsigned char)(((int16_t)me.rotation_fil[0])& 0x00ff);
				pWord[3]=(unsigned char)((int16_t)(realYSpeed)>>8 & 0x00ff);
				pWord[2]=(unsigned char)((int16_t)(realYSpeed) & 0x00ff);
				pWord[5]=(unsigned char)(((int16_t)RC_Ctl.key.forward)>>8 & 0x00ff);
				pWord[4]=(unsigned char)(((int16_t)RC_Ctl.key.forward)& 0x00ff);
				pWord[7]=(unsigned char)(((int16_t)setYSpeed)>>8 & 0x00ff); 
				pWord[6]=(unsigned char)(((int16_t)setYSpeed)& 0x00ff);
		   RS232_VisualScope( USART3, pWord, 8);
				
			i%=30000;
			
			
			PIDAlgorithm();
			
			BMotor_PWM(1);
				#ifndef Gun
			BMotor_PWM(2);
				#endif
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


		}else RM_SystemSwitch(0);
	}

return 0;
	
}



