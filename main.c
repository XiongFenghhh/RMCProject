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
//        i++;     //���һ�γ�ʼ��û�гɹ����Ǿ�����һ��                     
//        if(i>10) //�����ʼ��һֱ���ɹ����Ǿ�ûϣ���ˣ�������ѭ����������һֱ��
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
		i++;
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
	if(i==5000)getMouse();		
//	}
	}

return 0;
	
}



