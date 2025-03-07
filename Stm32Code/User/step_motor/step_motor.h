#ifndef __STEP_MOTOR_H
#define __STEP_MOTOR_H	 
#include "sys.h"

 
extern u8 STEP; 
 
 
#define STEP_MOTOR_PORT			GPIOA	//??IO?????
#define STEP_MOTOR_A				GPIO_Pin_2	//??IO??
#define STEP_MOTOR_B				GPIO_Pin_3	//??IO??
#define STEP_MOTOR_C				GPIO_Pin_4	//??IO??
#define STEP_MOTOR_D				GPIO_Pin_5	//??IO??
#define STEP_MOTOR_CLK      RCC_APB2Periph_GPIOA
 
 
void STEP_MOTOR_Init(void);//???
void STEP_MOTOR_OFF (void);//????
void STEP_MOTOR_8A (u8 a,u16 speed);
void STEP_MOTOR_NUM (u8 RL,u16 num,u8 speed);//???????
void STEP_MOTOR_LOOP (u8 RL,u8 LOOP,u8 speed);//???????
 
#endif