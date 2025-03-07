#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK精英STM32开发板
//IIC驱动 代码
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////

//初始化IIC
void IICSCL_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//使能GPIOB时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD ;   //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB,GPIO_Pin_5); 	//PB6,PB7 输出高
}
void IICSDA_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//使能GPIOB时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD ;   //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB,GPIO_Pin_6); 	//PB6,PB7 输出高
}
uint16_t IIC_Read_Byte(void)
{
    uint8_t rec_data;
    uint16_t rec_data16;
    I2C_Start();
    I2C_SendByte(0x54);
    I2C_WaitAck();
    I2C_SendByte(0xa1);
    I2C_WaitAck();
    I2C_Start();
    I2C_SendByte(0x55);
    I2C_WaitAck();
    rec_data = I2C_RecvByte();
    rec_data16=rec_data;
    I2C_SendACK(0);
    rec_data = I2C_RecvByte();
    rec_data16=rec_data16<<8|rec_data;
    I2C_Stop();
    return rec_data16;
}
/*起始信号*/
u8 I2C_Start(void)
{
    SDA1H;
    I2C_delay();
    SCL1H;
    I2C_delay();
    if(!SDAread)
        return 0;
    SDA1L;
    I2C_delay();
    if(SDAread)
        return 0;
    SCL1L;
    I2C_delay();
    return 1;
}
/*发送一个字节*/
void I2C_SendByte(u8 SendByte) //数据从高位到低位//
{
    u8 i=8;
    SCL1L;
    for (i=0; i<8; i++)         //8位计数器
    {
        if(SendByte&0x80)//SDA准备
            SDA1H;
        else SDA1L;
            
        SCL1H;                //拉高时钟，给从机采样
        I2C_delay();        //延时保持IIC时钟频率，也是给从机采样有充足时间
        SCL1L;                //拉低时钟，给SDA准备
        I2C_delay(); 		  //延时保持IIC时钟频率
        SendByte<<=1;         //移出数据的最高位
    }
}
/*等待ACK*/
u8 I2C_WaitAck(void)  //返回为:=1有ACK,=0无ACK
{
    uint16_t i=0;
    SDA1H;	        //释放SDA
    SCL1H;         //SCL拉高进行采样
    while(SDAread)//等待SDA拉低
    {
        i++;      //等待计数
        if(i==1000)//超时跳出循环
            break;
    }
    if(SDAread)//再次判断SDA是否拉低
    {
        SCL1L;
        return 0;//从机应答失败，返回0
    }
    I2C_delay();//延时保证时钟频率低于40K，
    SCL1L;
    I2C_delay(); //延时保证时钟频率低于40K，
    return 1;//从机应答成功，返回1
}
/*接受八位数据*/
u8 I2C_RecvByte(void) //数据从高位到低位//
{
    u8 i=8;
    u8 ReceiveByte=0;
    SDA1H; //释放SDA，给从机使用
    I2C_delay(); //延时给从机准备SDA时间
    for (i=0; i<8; i++)         //8位计数器
    {
        ReceiveByte <<= 1;
        SCL1H;                //拉高时钟线，采样从机SDA
        if(SDAread) //读数据
            ReceiveByte |=0x01;
        I2C_delay();     //延时保持IIC时钟频率
        SCL1L;           //拉低时钟线，处理接收到的数据
        I2C_delay();   //延时给从机准备SDA时间
    }
    return ReceiveByte;
}



/*结束信号*/
void I2C_Stop(void)
{
    SCL1L;
    I2C_delay();
    SDA1L;
    I2C_delay();
    SCL1H;
    I2C_delay();
    SDA1H;
    I2C_delay();
}
/*IIC等待延时*/
void I2C_delay(void)
{
    int x=5;
    u8 i=100;
    x=i*x;
    while(x--);
}
void I2C_SendACK(u8 i)
{
    if(i==1) SDA1H;
        
    else SDA1L;
        
    SCL1H;
    I2C_delay();
    SCL1L;
    I2C_delay();
}

















