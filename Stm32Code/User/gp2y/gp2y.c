
#include "sys.h"
/*
******ADC初始化*****************************************
*	函数功能:	GP2Y初始化
*	入口参数:	无
*	返回参数:	无
初始化ADC，这里我们仅以规则通道为例，我们默认将开启通道0~3	
**********************************************************
*/
void GP2Y_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1, ENABLE );	  //使能ADC1通道时钟
 

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M

	//PA1 作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	ADC_DeInit(ADC1);  //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   

   ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_239Cycles5);
    
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	
	ADC_ResetCalibration(ADC1);	//使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1);	 //开启AD校准
 
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
 
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能

}



void GP2Y_LED_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PB,PE端口时钟
    //配置PB7为输出模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;			//第9个引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//[可选]推挽输出模式，增加输出电流，但是也会增加功耗
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//引脚的工作速度，速度越快，功耗越高
    GPIO_Init(GPIOA, &GPIO_InitStructure);				//配置端口B
    GPIO_SetBits(GPIOA, GPIO_Pin_6);//置1
}


u16 GetGP2YSingleValue(void)
{
		int samplingTime = 280;//等待LED开启的时间是280μs
		uint16_t ADCVal;
		float dustVal = 0;
		float Voltage;
	
		GPIO_ResetBits(GPIOA, GPIO_Pin_6);//置1  开启内部LED  ????????
		delay_us(samplingTime); 					// 开启LED后的280us的等待时间
		
//		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
//    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
//		ADCVal = ADC_GetConversionValue(ADC1);
     ADCVal=Get_Adc(7);
		delay_us(40);
		GPIO_SetBits(GPIOA, GPIO_Pin_6);//置0
		delay_us(9680);//需要脉宽比0.32ms/10ms的PWM信号驱动传感器中的LED
		Voltage = ADCVal * 3.3 / 4096;
		dustVal = (Voltage + 1) * 1000 / 10;//*1000作用是将单位转换为ug/m3
//		if(ADCVal > 36.455)
//			dustVal = ((float)(ADCVal / 1024.0) - 0.0356) * 120000 * 0.035;
//		dustVal = 0.17 * ADCVal - 0.1;
//		dustVal = ADCVal;
//		return dustVal; //		mg/m3
    
		return (u16)dustVal;
}



