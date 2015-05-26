#ifndef RMVariables_h
#define RMVariables_h

#include "stm32f4xx.h"

#include "MotorEngine.h"
#include "BatholithTimer.h"
#include "RemoteController.h"
#include "Encoder.h"
#include "brushlessMotor.h"
#include "math.h"
#include "RMVhiecleSize.h"
#include "usart3.h"

#include "can.h"
#include "stepper.h"
#include "stdio.h"
#include "mpu6050_i2c.h"
#include "mpu6050_driver.h"
#include "mpu6050_process.h"
#include "mpu6050_interrupt.h"
#include "RMflash.h"

#define	BM_PWM_MAX	4000
//declaration of struct:MotorEngine and RC_Ctl_t
struct MotorEngine;
struct RC_Ctl_t;
struct __MPU6050_RAW_Data__;
struct __MPU6050_REAL_Data__;
typedef struct __MPU6050_RAW_Data__  MPU6050_RAW_DATA;
typedef struct __MPU6050_REAL_Data__  MPU6050_REAL_DATA;

//declaration of some global variables
extern struct MotorEngine me;
extern  MPU6050_RAW_DATA    MPU6050_Raw_Data; 
extern  MPU6050_REAL_DATA   MPU6050_Real_Data;




extern int16_t encoder_cnt[4];


void delay_ms(uint16_t);
/*
*Initialization of the whole project except MPU9150
*/
void RM_Init(void);
void RM_InterruptInit(void);

#endif
