#ifndef __RELAY_H__
#define __RELAY_H__

#include "sys.h"

#define RELAY_Execute         GPIO_Pin_7
#define RELAY_Execute3        GPIO_Pin_14
#define RELAY_Execute4        GPIO_Pin_13
#define RELAY_Execute6        GPIO_Pin_12

#define RELAY_GPIO_PORT  GPIOB
#define RELAY_GPIO_CLK  RCC_APB2Periph_GPIOB





#define Execute_ON        			GPIO_SetBits(GPIOA, RELAY_Execute)         
#define Execute_OFF      				GPIO_ResetBits(GPIOA, RELAY_Execute)          

#define Execute3_ON      			  GPIO_SetBits(RELAY_GPIO_PORT, RELAY_Execute3)         
#define Execute3_OFF      			GPIO_ResetBits(RELAY_GPIO_PORT, RELAY_Execute3) 

#define Execute4_ON       GPIO_SetBits(RELAY_GPIO_PORT, RELAY_Execute4)         
#define Execute4_OFF      GPIO_ResetBits(RELAY_GPIO_PORT, RELAY_Execute4) 

#define Execute6_ON       GPIO_SetBits(RELAY_GPIO_PORT, RELAY_Execute6)         
#define Execute6_OFF      GPIO_ResetBits(RELAY_GPIO_PORT, RELAY_Execute6) 

void RELAY_GPIO_Config(void);


#endif

