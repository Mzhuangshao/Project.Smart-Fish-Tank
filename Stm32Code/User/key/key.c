#include "stm32f10x.h"
#include "key.h"
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK精英STM32开发板
//按键驱动代码
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////

u8 isKey1 = 0;
u8 isKey2 = 0;
u8 isKey3 = 0;
u8 isKey4 = 0;
u8 isKey5 = 0;
u8 isKey6 = 0;
u8 isKey7 = 0;
u8 isKey8 = 0;

//
/*
******按键初始化函数*****************************************
*	功能:	按键初始化函数
*	入口参数:	无
*	返回参数:	无
*	说明:		KEY1-PB14		KEY2-PB12   KEY3-PB15    KEY4-PB13
************************************************************
*/
void KEY_Init(void) //IO初始化
{
 	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能PORTA时钟
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;//KEY0,KEY1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//
}


void Key1Press(void)
{
    if(isKey1 == 0)
        isKey1=1;
}

void Key2Press(void)
{
    if(isKey2 == 0)
        isKey2=1;

}

void Key3Press(void)
{
    if(isKey3 == 0)
        isKey3=1;

}

void Key4Press(void)
{
    if(isKey4 == 0)
        isKey4=1;

}

//void Key5Press(void)
//{
//    if(isKey5 == 0)
//        isKey5=1;
//}

//void Key6Press(void)
//{
//    if(isKey6 == 0)
//        isKey6=1;

//}

//void Key7Press(void)
//{
//    if(isKey7 == 0)
//        isKey7=1;

//}

//void Key8Press(void)
//{
//    if(isKey8 == 0)
//        isKey8=1;

//}

/*
******按键扫描函数*****************************************
*	功能:	按键扫描函数
*	入口参数:	无
*	返回参数:	无
*	说明:		KEY1-PB14		KEY2-PB12   KEY3-PB15    KEY4-PB13
************************************************************
*/
void KeyScan(void)
{
    static int keyCount = 0;
    static int keyState = 0;
    if (KEY1 == 0 && keyState == 0) //按键按下
    {
        keyCount++;
        if (keyCount >= 1 && KEY1 == 0 && keyState == 0) //加两次类似延迟10ms，不好解释
        {
            /*ToDo:按键按下执行的操作*/
            Key1Press();
            keyState = 1;
        }
    }
    else if (KEY2 == 0 && keyState == 0)
    {
        keyCount++;
        if (keyCount >= 1 && KEY2 == 0 && keyState == 0)
        {
            /*ToDo:按键按下执行的操作*/
            Key2Press();
            keyState = 1;
        }
    }
    else if (KEY3 == 0 && keyState == 0)
    {
        keyCount++;
        if (keyCount >= 1 && KEY3 == 0 && keyState == 0)
        {
            /*ToDo:按键按下执行的操作*/
            Key3Press();
            keyState = 1;
        }
    }
    else if (KEY4 == 0 && keyState == 0)
    {
        keyCount++;
        if (keyCount >= 1 && KEY4 == 0 && keyState == 0)
        {
            /*ToDo:按键按下执行的操作*/
            Key4Press();
            keyState = 1;
        }
    }

    
    else if (KEY1 == 1 && KEY2 == 1 && KEY3 == 1 && KEY4 == 1 &&keyState == 1) //当所有按键都处于抬起状态，状态刷新  && KEY5 == 1 && KEY6 == 1 && KEY7 == 1 && KEY8 == 1&&keyState == 
    {
        keyCount = 0;
        keyState = 0;
    }
}
