#include "app.h"
#define _load
#define GAP 0.0
/**
*@author Xiong Feng
*set Pitch and Yaw parameters of yuntai for debugging purpose
*/
 static double lp_p =25;
 static double lp_i = 0.0;
 static double lp_d = 60;
 static double ly_p = 4;
 static double ly_i = 0;
 static double ly_d =16;
 static double lf_p=0;
 static double lf_d=0;
 static double lf_i=0;
void setPitchPositionParameters(double kp,double ki,double kd){
lp_p=kp;
lp_i=ki;
lp_d=kd;
}

void setYawPositionParameters(double kp,double ki,double kd){
ly_p=kp;
ly_i=ki;
ly_d=kd;
}
FLASH_Status storeCurrentYuntaiParameters(void){
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGPERR | FLASH_FLAG_WRPERR);
	 if (FLASH_EraseSector(FLASH_Sector_23 , VoltageRange_3) != FLASH_COMPLETE)
    { 
      while (1)
      {
      }
    }
	if(storeYunTaiParameters(Yuntai_lp_kp_Address,lp_p)!=FLASH_COMPLETE){
	return FLASH_ERROR_PROGRAM;	
	};
	if(storeYunTaiParameters(Yuntai_lp_ki_Address,lp_i)!=FLASH_COMPLETE){
	return FLASH_ERROR_PROGRAM;	
	};
	if(storeYunTaiParameters(Yuntai_lp_ki_Address,lp_i)!=FLASH_COMPLETE){
	return FLASH_ERROR_PROGRAM;		
	};
	if(storeYunTaiParameters(Yuntai_ly_ki_Address,ly_p)!=FLASH_COMPLETE){
	return FLASH_ERROR_PROGRAM;		
	};
	if(storeYunTaiParameters(Yuntai_ly_ki_Address,ly_i)!=FLASH_COMPLETE){
	return FLASH_ERROR_PROGRAM;		
	};
	if(storeYunTaiParameters(Yuntai_ly_ki_Address,ly_d)!=FLASH_COMPLETE){
	return FLASH_ERROR_PROGRAM;		
	};
	FLASH_Lock();	
	return FLASH_COMPLETE;
}

void loadYuntaiParameters(void){
readYunTaiParameters(Yuntai_lp_kp_Address,&lp_p);
	readYunTaiParameters(Yuntai_lp_ki_Address,&lp_i);
	readYunTaiParameters(Yuntai_lp_kd_Address,&lp_d);
	readYunTaiParameters(Yuntai_ly_kp_Address,&ly_p);
	readYunTaiParameters(Yuntai_ly_ki_Address,&ly_i);
	readYunTaiParameters(Yuntai_ly_kd_Address,&ly_d);
}
static int outputCount=0;
void  outputData(void){
uint8_t i;
uint8_t temp8[10];
uint16_t temp16[4];
uint8_t t;
temp16[0]=0;//(uint16_t)ly_p;
temp16[1]=0;//(uint16_t)ly_i;
temp16[2]=0;//(uint16_t)ly_d;	
temp16[3]=0;
temp8[0]=0x7e;
for(i=0;i<4;i++){
temp8[i*2+1]=(uint8_t)(temp16[i]%256);
temp8[i*2+2]=(uint8_t)(temp16[i]/256);
}


temp8[9]=0x7e;

for(outputCount=0;outputCount<10;outputCount++){
	USART3_SendChar(temp8[outputCount]);
}
outputCount=0;
}
/********************************************************************************
   ������巢��ָ�ID��Ϊ0x200��ֻ����������壬���ݻش�IDΪ0x201��0x202
	 cyq:����Ϊ�������������ָ�
*********************************************************************************/
void Cmd_ESC(int16_t current_201,int16_t current_202,int16_t current_203)
{
    CanTxMsg tx_message;
    
    tx_message.StdId = 0x200;
    tx_message.IDE = CAN_Id_Standard;
    tx_message.RTR = CAN_RTR_Data;
    tx_message.DLC = 0x08;
    
    tx_message.Data[0] = (unsigned char)(current_201 >> 8);
    tx_message.Data[1] = (unsigned char)current_201;
    tx_message.Data[2] = (unsigned char)(current_202 >> 8);
    tx_message.Data[3] = (unsigned char)current_202;
    tx_message.Data[4] = (unsigned char)(current_203 >> 8);
    tx_message.Data[5] = (unsigned char)current_203;
    tx_message.Data[6] = 0x00;
    tx_message.Data[7] = 0x00;
    
    CAN_Transmit(CAN1,&tx_message);
}

/********************************************************************************
                         pitch��������ٶȻ�����
                    ���� pitch�ᵱǰ�ٶ� pitch��Ŀ���ٶ�
*********************************************************************************/
double Velocity_Control_201(double current_velocity_201,double target_velocity_201)
{
    const double v_p = 50.0;
    const double v_d = 0;
    
    static double error_v[2] = {0.0,0.0};
    static double output = 0;
    
    if(abs(current_velocity_201) < GAP)
    {
        current_velocity_201 = 0.0;
    }
    
    error_v[0] = error_v[1];
    error_v[1] = target_velocity_201 - current_velocity_201;
    
    output = error_v[1] * v_p             
             + (error_v[1] - error_v[0]) * v_d;
     
    if(output > ESC_MAX)
    {
        output = ESC_MAX;
    }
    
    if(output < -ESC_MAX)
    {
        output = -ESC_MAX;
    }
    
    return -output;//cyq:for6015 ����
}


/********************************************************************************
                         pitch�������λ�û����� 
                    ���� pitch�ᵱǰλ�� pitch��Ŀ��λ��
*********************************************************************************/
double Position_Control_201(double current_position_201,double target_position_201)
{
    
    

    static double error_l[2] = {0.0,0.0};
    static double output = 0;
    static double inte = 0;
    
    error_l[0] = error_l[1];
    error_l[1] = target_position_201 - current_position_201;
    inte += error_l[1]; 
    
    output = error_l[1] * lp_p 
            + inte * lp_i 
            + (error_l[1] - error_l[0]) * lp_d;
    //output-=current_position_201*lp_d;
    if(output > ESC_MAX)
    {
        output = ESC_MAX;
    }
    
    if(output < -ESC_MAX)
    {
        output = -ESC_MAX;
    }
    		
    return output;
}
/********************************************************************************
                           yaw��������ٶȻ�����
                      ���� yaw�ᵱǰ�ٶ� yaw��Ŀ���ٶ�
*********************************************************************************/
double Velocity_Control_203(double current_velocity_203,double target_velocity_203)
{
    const double v_p = 50.0;
    const double v_d = 0.0;
    
    static double error_v[2] = {0.0,0.0};
    static double output = 0;
		
    if(abs(current_velocity_203) < GAP)
    {
        current_velocity_203 = 0.0;
    }
    
    error_v[0] = error_v[1];
    error_v[1] = target_velocity_203 - current_velocity_203;
    
    output = error_v[1] * v_p
             + (error_v[1] - error_v[0]) * v_d;
     
    if(output > ESC_MAX)
    {
        output = ESC_MAX;
    }
    
    if(output < -ESC_MAX)
    {
        output = -ESC_MAX;
    }
    
    return -output;//cyq:for6015 ����
}

/********************************************************************************
                           yaw�������λ�û�����
                      ���� yaw�ᵱǰλ�� yaw��Ŀ��λ��
@modified by huangmin on 2015.04.25
@fixed the positive feedback bug ��not return -output
@zero_crossing strategy
@TODO optimal control(already got the idea)
*********************************************************************************/
double Position_Control_203(double current_position_203,double target_position_203,uint16_t isMoving)
{
    static double error_l[3] = {0.0,0.0,0.0};
    static double output = 0;
    
//		if(abs(current_position_203-target_position_203)>100)l_p=14.5;
//		else if(abs(current_position_203-target_position_203)<70) l_p=12;
		if(isMoving==1){
//			
//			l_p=30;
		}
		error_l[0] = error_l[1];
      error_l[1] = error_l[2]; 
		  error_l[2] = target_position_203 - current_position_203;
    
			//if(error_l[2]>300){l_p=27;l_d=0.03;}
	//	l_p=10*((abs(error_l[2]))/250)*((abs(error_l[2]))/250)+8;
    output = error_l[2] * ly_p 
							+ (error_l[0]*0.2+error_l[2]*0.4+error_l[1]*0.3)  * ly_i 
			+ (error_l[2] - error_l[1]) * ly_d;//@TODO: i controll has no effect on output
    
    if(output > ESC_MAX)
    {
        output = ESC_MAX;
    }
    
    if(output < -ESC_MAX)
    {
        output = -ESC_MAX;
    }
	  return output;
}
/********************************************************************************
@ modified by huangmin on 2015.04.25
@ added current feedback function of yaw motor 
*********************************************************************************/
double Current_Control_203(double current_203,double target_current_203)
{
	  const double c_p = 2.0;
	  const double c_i = 0.001;
	  const double c_d = 0.00;
	
    static double error_c[3] = {0.0,0.0,0.0};
    static double output = 0;
    
    error_c[0] = error_c[1];
    error_c[1] = error_c[2];    
    error_c[2] = target_current_203 + current_203/13.0*5.0;
 
    output = error_c[2] * c_p 
							+ (error_c[2]+error_c[1]+error_c[0]) * c_i 
							+ (error_c[2] - error_c[1]) * c_d;
    
    if(output > ESC_MAX)
    {
        output = ESC_MAX;
    }
    
    if(output < -ESC_MAX)
    {
        output = -ESC_MAX;
    }
    
    return output;
}

/********************************************************************************
@ modified by huangmin on 2015.04.25
@ added current feedback function of yaw motor 
*********************************************************************************/
double followControl(int current_position_203)
{
	static double error_l[3] = {0.0,0.0,0.0};
    static double output = 0;
    
//		if(abs(current_position_203-target_position_203)>100)l_p=14.5;
//		else if(abs(current_position_203-target_position_203)<70) l_p=12;
		
		error_l[0] = error_l[1];
      error_l[1] = error_l[2]; 
		  error_l[2] = 3200 - current_position_203;
    

    output = error_l[2] * lf_p 
							+ (error_l[0]*0.2+error_l[2]*0.4+error_l[1]*0.3)  * lf_i 
			+ (error_l[2] - error_l[1]) * lf_d;//@TODO: i controll has no effect on output
    
    if(output > 2)
    {
        output = 2;
    }
    
    if(output < -2)
    {
        output = -2;
    }
	  return output;
}

