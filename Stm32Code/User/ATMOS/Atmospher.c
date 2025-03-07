
//#include "bsp_oled_iic.h"
//#include <stdio.h>
//#include <string.h> // 添加包含 string.h 头文件
//#include "usart3.h"
//#include "delay.h"
//#include "sys.h"
#include "Atmospher.h"

//所用压力传感器型号：	HX711

//比例系数确定数据：
//	42500 	-->  160g --> 265.65
//	80000 	-->  285g --> 280.7
//	115000 	-->  405g --> 283.95
//	405000 	--> 1400g --> 289.29
//拟合直线：
// y = 291.92x-3580.2 (忽略截距)

float Kp_Press = 293.0f;


unsigned long ATMOS_Buffer = 0;
unsigned long Press_No_Lode = 0;
unsigned long Press_Lode = 0;

float Press_Real = 0;
float PressMin=0.0f;



void Atmospher_Init(void)
{
	GPIO_InitTypeDef	gpio;  

	RCC_APB2PeriphClockCmd(Sensor_Clock,ENABLE);
	
	//时钟线推挽输出
	gpio.GPIO_Pin = CLK;	
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Sensor_Gpio,&gpio);

	//数据线浮空输入
	gpio.GPIO_Pin = OUT;	
	gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Sensor_Gpio,&gpio);
}


unsigned long Atmospher_Read(void)
{
	unsigned long value;
	unsigned char i;
	
	//每次读取数据前保证数据线电平稳定
	//此处只是为了稳定电平 拉高或拉低效果相同
//	GPIO_ResetBits(Sensor_Gpio,DATA);
	GPIO_SetBits(Sensor_Gpio,OUT);
	
	//为了等待输出电平稳定
	//在每次一操作电平时加微小延时
	delay_us(2);
	
	//时钟线拉低 空闲时时钟线保持低电位
	GPIO_ResetBits(Sensor_Gpio,CLK);
	
	delay_us(2);	
	
	//等待AD转换结束
	while(GPIO_ReadInputDataBit(Sensor_Gpio,OUT));
	
	for(i=0;i<24;i++)
	{
		//时钟线拉高 开始发送时钟脉冲
		GPIO_SetBits(Sensor_Gpio,CLK);
		
		delay_us(2);
		
		//左移位 右侧补零 等待接收数据
		value = value << 1;
		
		//时钟线拉低
		GPIO_ResetBits(Sensor_Gpio,CLK);
		
		delay_us(2);
		
		//读取一位数据
		if(GPIO_ReadInputDataBit(Sensor_Gpio,OUT))
			value ++;
	}
	
	//第25个脉冲
	GPIO_SetBits(Sensor_Gpio,CLK);
	
	delay_us(2);

	//第25个脉冲下降沿到来时 转换数据
	//此处说明：
	//			HX711是一款24位的AD转换芯片
	//			最高位是符号位 其余为有效位
	//			输出数组最小值0x800000
	//					最大值0x7FFFFF
	//异或运算：
	//			相同为0 
	//			不同为1
	//数据处理说明：
	//			之所以会发生 INPA-INNA < 0mv 的情况
	//			是因为发生了零点漂移
	//			例如上面的数据就是初始状态INPA-INNA = -0.5mv
	//			然后随着重量的增加会发生过零点
	//			这时如果直接使用读取到的数据就会发生错误
	//			因为读取到的是小于0的二进制补码
	//			是不能直接使用的 需要转换成其原码

	//			比较简单的处理方法就是读到的数据直接和0x800000进行异或
	//			这时最高位可以看做是有效位
	//			不代表符号位而代表的下一位的进位
	//			这样数据会一直往上增长 
	//			我们可以直接拿来进行使用
	value = value^0x800000;
//	value = value&0x7FFFFF;
	
	//第25个脉冲结束
	GPIO_ResetBits(Sensor_Gpio,CLK);
	
	delay_us(2);
	
	return value;
}


void Get_No_Press(void)
{
	ATMOS_Buffer = Atmospher_Read();
	
	Press_No_Lode = ATMOS_Buffer;
	
//	No_Load_Flag = 1;
}


void Get_Press(void)
{ 
    
	ATMOS_Buffer = Atmospher_Read();
	
	Press_Lode = ATMOS_Buffer;
		
	//判断非空载
	if(Press_Lode > Press_No_Lode)
	{
		Press_Real = (Press_Lode - Press_No_Lode)/Kp_Press;
	}
	else if(Press_Lode <= Press_No_Lode)
		Press_Real = 0.0f;
    
	SensorData.Atmospher=Press_Real;
	//拉力达到一定阈值 则串口显示
//    sprintf((char*)Tx_Weight,"%4.2f   ",Weight_Real);
    
//    sprintf((char*)str,"%4.2f  ",Weight_Real);
//    oled_ShowString(0,0,"Weight:",16);
//    oled_ShowString(64,0,str,16);
//    oled_ShowString(120,0,"g",16);

//    oled_ShowString(0,2,"Person:",16);
//    oled_ShowNum(64,2,Person,2,16);

}