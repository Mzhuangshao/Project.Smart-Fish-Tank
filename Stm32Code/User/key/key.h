#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
  	 

extern u8 isKey1;
extern u8 isKey2;
extern u8 isKey3;
extern u8 isKey4;
extern u8 isKey5;
extern u8 isKey6;
extern u8 isKey7;
extern u8 isKey8;

/* 相关端口配置宏 */
#define KEY1_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define KEY1_GPIO_PORT    	GPIOB						/* GPIO端口 */
#define KEY1_GPIO_PIN		    GPIO_Pin_5					/* 连接到SCL时钟线的GPIO */


#define KEY2_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define KEY2_GPIO_PORT    	GPIOB						/* GPIO端口 */
#define KEY2_GPIO_PIN		    GPIO_Pin_6					/* 连接到SCL时钟线的GPIO */

#define KEY3_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define KEY3_GPIO_PORT    	GPIOB						/* GPIO端口 */
#define KEY3_GPIO_PIN		    GPIO_Pin_7					/* 连接到SCL时钟线的GPIO */

#define KEY4_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define KEY4_GPIO_PORT    	GPIOB						/* GPIO端口 */
#define KEY4_GPIO_PIN		    GPIO_Pin_8					/* 连接到SCL时钟线的GPIO */







#define KEY1   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)
#define KEY2   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)
#define KEY3   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)
#define KEY4   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)




void KEY_Init(void);//IO初始化
void Key1Press(void);
void Key2Press(void);
void Key3Press(void);
void Key4Press(void);


void KeyScan(void);
#endif
