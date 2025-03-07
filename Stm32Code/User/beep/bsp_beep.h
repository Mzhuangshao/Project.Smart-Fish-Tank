#ifndef __BEEP_H
#define	__BEEP_H


#include "sys.h" 


/* 相关端口配置宏 */
#define Execute7_GPIO_CLK 	    RCC_APB2Periph_GPIOA		/* GPIO端口时钟 */
#define Execute7_GPIO_PORT    	GPIOA						/* GPIO端口 */
#define Execute7_GPIO_PIN		    GPIO_Pin_8					/* 连接到SCL时钟线的GPIO */








/* 定义控制IO的宏 */
#define Execute7_TOGGLE		    digitalToggle(Execute7_GPIO_PORT,Execute7_GPIO_PIN)
#define Execute7_ON		       digitalHi(Execute7_GPIO_PORT,Execute7_GPIO_PIN)
#define Execute7_OFF			   digitalLo(Execute7_GPIO_PORT,Execute7_GPIO_PIN)

void Beep_Init(void);
//void Beep_StateRefresh(uint8_t BeepState); 





#endif 
