#include "sys.h"

SENSOR SensorData;   // 传感器结构体定义
THRESHOLD Threshold; // 阈值结构体定义
SYSTEM System;       // 系统标志位结构体定义

void Threshold_Init(THRESHOLD *Threshold);

int main(void)
{
    delay_init(); // 延时函数初始化

    NVIC_Config(); // 中断优先级配置
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    //    My_USART1();//串口1初始化 （调试打印用） 波特率：9600

    RELAY_GPIO_Config(); // 继电器初始化
    Adc_Init();          // adc初始化，读取传感器值

    Beep_Init(); // 蜂鸣器初始化
    SG90_Init(); // 舵机
    DS18B20_Init();
    LED_GPIO_Config(); // LED灯初始化
    oled_Init();       // oled初始化
    TIM1_Int_Init(10000 - 1, 7200 - 1);
    //	   TIM2_Init(5000-1,7200-1);
    KEY_Init();                 // 按键初始化
    Threshold_Init(&Threshold); // 传感器阈值及执行器件开关变量初始化
    oled_Clear();

    //    oled_ShowString(0,2,"Connecting...",16);
    /*******************************************/
    while (1)
    {

        KeyScan();
        //        SensorData.Sensor1 = 100-Get_Adc_Average(0, 10) / 41; //浑浊度
        //        SensorData.Sensor2 = 100 - Get_Adc_Average(4, 10) / 41; //获取光照强度;
        SensorData.Sensor3 = (int)DS18B20_GetTemperture(); // 温度
        SensorData.Sensor4 = Get_Adc_Average(6, 10) / 41;  // 水位
        Mode_Decide();                                     // 模式判定 （按键1选择模式）
    }
}

// 传感器阈值及执行器件开关变量初始化
void Threshold_Init(THRESHOLD *Threshold)
{

    Threshold->Sensor1MAX = 50; // 浊度
    Threshold->Sensor2MAX = 50; // 光照
    Threshold->Sensor3MAX = 20; // 温度
    Threshold->Sensor4MAX = 20; // 水位
}
