#ifndef __ATMOSPHER_H__
#define __ATMOSPHER_H__

#include "sys.h"

//宏定义
#define Sensor_Gpio GPIOA

#define OUT GPIO_Pin_7
#define CLK  GPIO_Pin_6

#define Sensor_Clock RCC_APB2Periph_GPIOA


void Atmospher_Init(void);
unsigned long Atmospher_Read(void);

void Get_No_Press(void);
void Get_Press(void);


#endif

