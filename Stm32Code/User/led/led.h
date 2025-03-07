#ifndef __LED_H__
#define __LED_H__

#include "sys.h"



//宏定义灯使用的io端口初始化配置
#define Execute2_GPIO            GPIO_Pin_13
#define Execute2_GPIO_PORT       GPIOC
#define Execute2_GPIO_CLK        RCC_APB2Periph_GPIOC


#define WIFI_GPIO_PORT       GPIOA
#define WIFI_GPIO_CLK       RCC_APB2Periph_GPIOA
#define wifi_rst_PIN        GPIO_Pin_1

#define Execute2_OFF       GPIO_ResetBits(Execute2_GPIO_PORT, Execute2_GPIO) //关        
#define Execute2_ON        GPIO_SetBits(Execute2_GPIO_PORT, Execute2_GPIO)      //开

#define WIFI_RST     GPIO_SetBits(WIFI_GPIO_PORT, wifi_rst_PIN) 

void LED_GPIO_Config(void);
void wifi_GPIO_Config(void);

#endif

