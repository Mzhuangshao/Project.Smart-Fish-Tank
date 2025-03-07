#include "sys.h"                  // Device header



uint16_t T;
uint16_t Time;

void HCSR04_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(Trig_RCC, ENABLE);
	
	
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = Trig_Pin;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Trig_Port, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStruct.GPIO_Pin = Echo_Pin;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Echo_Port, &GPIO_InitStruct);
	
	GPIO_ResetBits(Trig_Port, Trig_Pin);
	
}

void HCSR04_Start()
{
	GPIO_SetBits(Trig_Port, Trig_Pin);
	delay_us(45);
	GPIO_ResetBits(Trig_Port, Trig_Pin);
//	Timer_Init();需要打开
}

uint16_t HCSR04_GetValue()
{
	HCSR04_Start();
	delay_ms(100);
	return ((Time * 0.0001) * 34000) / 2;
//	return Time;
}


void get_HCSR04(){
			T = HCSR04_GetValue();
		
	oled_ShowNum(16*4,2*1,T,3,16);
		delay_ms(100);
}


