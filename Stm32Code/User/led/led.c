
#include "sys.h"

void LED_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(Execute2_GPIO_CLK, ENABLE);
	GPIO_InitStruct.GPIO_Pin = Execute2_GPIO ;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Execute2_GPIO_PORT, &GPIO_InitStruct);	
  Execute2_OFF;		
}
void wifi_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(WIFI_GPIO_CLK, ENABLE);

	GPIO_InitStruct.GPIO_Pin = wifi_rst_PIN ;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(WIFI_GPIO_PORT, &GPIO_InitStruct);	
	
   GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_RESET);
	 delay_ms(250);
	GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET);
	delay_ms(500);
	
}





