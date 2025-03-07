#ifndef __MEMS_H
#define __MEMS_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//IIC驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//IO方向设置
 
#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}

//IO操作函数	 
#define IIC_SCL    PBout(6) //SCL
#define IIC_SDA    PBout(5) //SDA	 
#define SDAread   PBin(5)  //输入SDA 

#define SCL1L GPIO_ResetBits(GPIOB,GPIO_Pin_6)
#define SCL1H GPIO_SetBits(GPIOB,GPIO_Pin_6)

#define SDA1L GPIO_ResetBits(GPIOB,GPIO_Pin_5)
#define SDA1H GPIO_SetBits(GPIOB,GPIO_Pin_5)


#define slaveID 0xd8
#define trycount 5

extern volatile unsigned char presshigh,presslow;

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口
uint16_t IIC_Read_Byte(void);
u8 I2C_Start(void);
void I2C_SendByte(u8 SendByte); //数据从高位到低位//
u8 I2C_WaitAck(void);  //返回为:=1有ACK,=0无ACK
u8 I2C_RecvByte(void); //数据从高位到低位//
void I2C_Stop(void);
void I2C_delay(void);
 void IICSDA_Init(void);
  void IICSCL_Init(void);
void I2C_SendACK(u8 i);
#endif
















