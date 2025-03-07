#ifndef __HX711_H
#define __HX711_H

#include "sys.h"

//宏定义
#define AtmosGPIO GPIOB

#define DATA GPIO_Pin_0
#define AtmosCLK  GPIO_Pin_1

#define AtmosClock RCC_APB2Periph_GPIOB

extern float WeightMin;
extern float Weight_Real;
extern uint8_t Tx_Weight[30];
extern uint8_t Person;
void Sensor_Init(void);
unsigned long Sensor_Read(void);
void test_mode(void);
void Get_No_Lode(void);
void Get_Weight(void);

#endif


