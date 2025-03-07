#include "sys.h"

uint8_t OperateMode = 0; // 运行模式标志位
uint8_t IndexFlag1 = 1;  // 选项索引
uint8_t CursorFlag1 = 1; // 光标索引
uint8_t IndexFlag = 1;   // 选项索引
uint8_t CursorFlag = 1;  // 光标索引
// 投喂时间标志位
uint8_t TimeFlag = 1;
uint8_t TimeFlag1 = 0;
// 增氧时间标志位
uint8_t STimeFlag = 0;
uint8_t STimeFlag1 = 0;
// 增氧和投喂时间切换
uint8_t GoTimeFlag = 0;
// 投喂 增氧时间
uint8_t GoFoodTime = 0;
uint8_t GoOxTime = 0;
// 投喂 增氧
uint8_t GoFood = 0;
uint8_t GoOx = 0;
/******************
功能：模式选择及控制界面
参数：无
返回值：无
******************/
void Mode_Decide(void) // 模式判定
{
    Manual_Mode();

    // 按键判定
    if (isKey1)
    {                                          // 若按键1按下
        isKey1 = isKey2 = isKey3 = isKey4 = 0; // 清除按键按下标志
        oled_Clear();
        OperateMode++;
    }
    //		else if (isKey2 && (!OperateMode)) { //若按键2按下,且当前模式为自动模式
    //        isKey1 = isKey2 = isKey3 = isKey4 = 0; //清除按键按下标志
    //        oled_Clear();
    //        InterFace = !InterFace;
    //    }

    // 标志位限制
    if (OperateMode >= 4)
    {
        OperateMode = 0;
    }

    /**根据模式选择显示界面**/
    switch (OperateMode)
    {
    case 0:
        Inform_Show(); // 显示信息
        AutoContrl();  // 自动控制
        break;

    case 1:
        Timed_mode(); // 定时
        AutoContrl(); // 自动控制
        break;

    case 2:
        AutoContrl();   // 自动控制
        ThresholdSet(); // 阈值设置1
        break;

    case 3:
        Manual_Contrl(); // 手动模式
        break;
    }
}

/******************
功能：显示界面
参数：无
返回值：无
******************/
void Inform_Show(void)

{
    oled_ShowCHinese(16 * 1, 2 * 0, 48);
    oled_ShowCHinese(16 * 2, 2 * 0, 49);
    oled_ShowCHinese(16 * 3, 2 * 0, 50);
    oled_ShowCHinese(16 * 4, 2 * 0, 51);
    oled_ShowCHinese(16 * 5, 2 * 0, 52);
    oled_ShowCHinese(16 * 6, 2 * 0, 53);

    // 第三行	温度
    oled_ShowCHinese(16 * 0, 2 * 1, 8);
    oled_ShowCHinese(16 * 1, 2 * 1, 9);
    oled_ShowString(16 * 4, 2 * 1, ":", 16);
    oled_ShowNum(16 * 5, 2 * 1, SensorData.Sensor3, 2, 16);
    oled_ShowCHinese(16 * 7, 2 * 1, 10);

    oled_ShowCHinese(16 * 0, 2 * 2, 11);
    oled_ShowCHinese(16 * 1, 2 * 2, 12);
    oled_ShowString(16 * 4, 2 * 2, ":", 16);
    oled_ShowNum(16 * 5, 2 * 2, SensorData.Sensor4, 2, 16);
    oled_ShowString(16 * 7, 2 * 2, "%", 16);
}

/******************
功能：自动控制
参数：无
返回值：无
******************/

void AutoContrl(void)
{
    // 定时时间到达后执行逻辑
    if (GoFood == 1)
        System.Execute5 = 1;
    else
        System.Execute5 = 0;

    //	if(GoOx==1)  System.Execute6=1;
    //  else System.Execute6=0;

    //	//浑浊度高于阈值 自动换水
    //	if(SensorData.Sensor1>=Threshold.Sensor1MAX)  System.Execute=1;
    //	else  System.Execute=0;
    ////光照低于阈值自动照明
    //		if(SensorData.Sensor2<=Threshold.Sensor2MAX)  System.Execute2=1;
    //	else  System.Execute2=0;

    // 温度低于阈值  自动加热
    if (SensorData.Sensor3 <= Threshold.Sensor3MAX)
        System.Execute3 = 1;
    else
        System.Execute3 = 0;

    // 水位低于阈值自动加水
    if (SensorData.Sensor4 <= Threshold.Sensor4MAX)
        System.Execute4 = 1;
    else
        System.Execute4 = 0;

    if (SensorData.Sensor3 <= Threshold.Sensor3MAX || SensorData.Sensor4 <= Threshold.Sensor4MAX)
    {
        System.Execute7 = 1;
    }
    else
    {
        System.Execute7 = 0;
    }
}

/******************
功能：定时模式
参数：无
返回值：无
******************/
void Timed_mode()
{
    oled_ShowCHinese(16 * 2, 2 * 0, 58);
    oled_ShowCHinese(16 * 3, 2 * 0, 59);
    // 按下选中
    if (isKey2)
    {
        TimeFlag++;

        if (TimeFlag > 4)
        {
            TimeFlag = 1;
        }

        isKey2 = 0;
    }

    // 选择设置投喂还是增氧
    //     if (TimeFlag == 0) {
    //         oled_ShowString(16 * 7, 2 * 0, "*", 16);

    ////        if (isKey3) {
    ////            GoTimeFlag=!GoTimeFlag;
    ////
    ////            isKey3 = 0;
    ////        }
    //    } else {
    //        oled_ShowString(16 * 7, 2 * 0, "  ", 16);
    //    }

    // 显示投喂还是增氧
    //     if (GoTimeFlag == 0) {
    oled_ShowCHinese(16 * 4, 2 * 0, 60);
    oled_ShowCHinese(16 * 5, 2 * 0, 61);
    //    }
    //		if (GoTimeFlag == 1)
    //		{
    //        oled_ShowCHinese(16 * 4, 2 * 0, 13);
    //        oled_ShowCHinese(16 * 5, 2 * 0, 14);
    //    }

    // 如果是喂食设置
    if (GoTimeFlag == 0)
    {

        // 投喂
        oled_ShowCHinese(16 * 0, 2 * 1, 25);
        oled_ShowCHinese(16 * 1, 2 * 1, 26);
        oled_ShowString(16 * 2, 2 * 1, ":", 16);
        oled_ShowNum(16 * 3, 2 * 1, SensorData.Sensor5, 2, 16);
        oled_ShowCHinese(16 * 4, 2 * 1, 26);
        oled_ShowNum(16 * 5, 2 * 1, SensorData.Sensor6, 2, 16);
        oled_ShowCHinese(16 * 6, 2 * 1, 27);
        oled_ShowNum(16 * 7, 2 * 1, SensorData.Sensor7, 2, 16);
        oled_ShowCHinese(16 * 0, 2 * 3, 19);
        oled_ShowCHinese(16 * 1, 2 * 3, 20);
        oled_ShowString(16 * 2, 2 * 3, ":", 16);

        oled_ShowNum(16 * 0, 2 * 2, GoFoodTime, 2, 16);

        // 设置时
        if (TimeFlag == 1)
        {
            oled_ShowString(16 * 3, 2 * 2, "--", 16);

            if (isKey3)
            {
                SensorData.Sensor5++;
                isKey3 = 0;
            }

            if (isKey4)
            {
                SensorData.Sensor5--;
                isKey4 = 0;
            }
        }
        else
        {
            oled_ShowString(16 * 3, 2 * 2, "  ", 16);
        }

        // 设置分
        if (TimeFlag == 2)
        {
            oled_ShowString(16 * 5, 2 * 2, "--", 16);

            if (isKey3)
            {
                SensorData.Sensor6++;
                isKey3 = 0;
            }

            if (isKey4)
            {
                SensorData.Sensor6--;
                isKey4 = 0;
            }
        }
        else
        {
            oled_ShowString(16 * 5, 2 * 2, "  ", 16);
        }
        // 喂食时间
        if (TimeFlag == 3)
        {
            oled_ShowString(16 * 7, 2 * 2, "*", 16);
            if (isKey3)
            {
                GoFoodTime++;
                isKey3 = 0;
            }
            if (isKey4)
            {
                GoFoodTime--;
                isKey4 = 0;
            }
        }
        else
            oled_ShowString(16 * 7, 2 * 2, "  ", 16);

        // 开关
        if (TimeFlag == 4)
        {
            oled_ShowString(16 * 7, 2 * 3, "*", 16);

            if (isKey3)
            {
                TimeFlag1 = !TimeFlag1;
                isKey3 = 0;
            }

            if (isKey4)
            {
                SensorData.Sensor5 = 0;
                SensorData.Sensor6 = 0;
                SensorData.Sensor7 = 0;
                GoFoodTime = 0;
                isKey4 = 0;
            }
        }
        else
        {
            oled_ShowString(16 * 7, 2 * 3, "  ", 16);
        }

        // 显示开关
        if (TimeFlag1 == 1)
        {
            oled_ShowCHinese(16 * 3, 2 * 3, 21);
            oled_ShowCHinese(16 * 4, 2 * 3, 22);
        }
        else
        {
            oled_ShowCHinese(16 * 3, 2 * 3, 23);
            oled_ShowCHinese(16 * 4, 2 * 3, 24);
        }
    }

    // 如果是增氧设置
    //		 if(GoTimeFlag==1){
    //
    //
    //			 		    //投喂
    //    oled_ShowCHinese(16 * 0, 2 * 1, 25);
    //    oled_ShowCHinese(16 * 1, 2 * 1, 26);
    //    oled_ShowString(16 * 2, 2 * 1, ":", 16);
    //    oled_ShowNum(16 * 3, 2 * 1, SensorData.Sensor8, 2, 16);
    //    oled_ShowCHinese(16 * 4, 2 * 1, 26);
    //    oled_ShowNum(16 * 5, 2 * 1, SensorData.Sensor9, 2, 16);
    //    oled_ShowCHinese(16 * 6, 2 * 1, 27);
    //    oled_ShowNum(16 * 7, 2 * 1, SensorData.Sensor10, 2, 16);
    //    oled_ShowCHinese(16 * 0, 2 * 3, 19);
    //    oled_ShowCHinese(16 * 1, 2 * 3, 20);
    //    oled_ShowString(16 * 2, 2 * 3, ":", 16);
    //
    //		 oled_ShowNum(16 * 0, 2 * 2, GoOxTime, 2, 16);
    //
    //
    //			   //设置时
    //    if (TimeFlag == 1) {
    //        oled_ShowString(16 * 3, 2 * 2, "--", 16);

    //        if (isKey3) {
    //            SensorData.Sensor8++;
    //            isKey3 = 0;
    //        }

    //        if (isKey4) {
    //            SensorData.Sensor8--;
    //            isKey4 = 0;
    //        }
    //    } else {
    //        oled_ShowString(16 * 3, 2 * 2, "  ", 16);
    //    }

    //    //设置分
    //    if (TimeFlag == 2) {
    //        oled_ShowString(16 * 5, 2 * 2, "--", 16);

    //        if (isKey3) {
    //            SensorData.Sensor9++;
    //            isKey3 = 0;
    //        }

    //        if (isKey4) {
    //            SensorData.Sensor9--;
    //            isKey4 = 0;
    //        }
    //    } else {
    //        oled_ShowString(16 * 5, 2 * 2, "  ", 16);
    //    }
    //   //增氧时间
    //		if(TimeFlag==3){
    //		  oled_ShowString(16 * 7, 2 * 2, "*", 16);
    //			 if (isKey3) {
    //           GoOxTime++;
    //            isKey3 = 0;
    //        }
    //						 if (isKey4) {
    //           GoOxTime--;
    //            isKey4 = 0;
    //        }
    //
    //		}else   oled_ShowString(16 * 7, 2 * 2, "  ", 16);

    //
    //
    //    //开关
    //    if (TimeFlag == 4) {
    //        oled_ShowString(16 * 7, 2 * 3, "*", 16);

    //        if (isKey3) {
    //            STimeFlag1 = !STimeFlag1;
    //            isKey3 = 0;
    //        }

    //        if (isKey4) {
    //            SensorData.Sensor8 = 0;
    //            SensorData.Sensor9 = 0;
    //            SensorData.Sensor10 = 0;
    //					  GoOxTime=0;
    //            isKey4 = 0;
    //        }
    //    }	else {
    //        oled_ShowString(16 * 7, 2 * 3, "  ", 16);
    //    }
    //
    //		//显示开关
    //		    if (STimeFlag1 == 1) {
    //        oled_ShowCHinese(16 * 3, 2 * 3, 21);
    //        oled_ShowCHinese(16 * 4, 2 * 3, 22);
    //    } else {
    //        oled_ShowCHinese(16 * 3, 2 * 3, 23);
    //        oled_ShowCHinese(16 * 4, 2 * 3, 24);
    //    }
    //
    //
    //	}
    //

    if (SensorData.Sensor5 > 24)
    {
        SensorData.Sensor5 = 0;
    }
    if (SensorData.Sensor6 > 60)
    {
        SensorData.Sensor6 = 0;
    }
    if (SensorData.Sensor7 > 60)
    {
        SensorData.Sensor7 = 0;
    }

    if (SensorData.Sensor8 > 24)
    {
        SensorData.Sensor8 = 0;
    }
    if (SensorData.Sensor9 > 60)
    {
        SensorData.Sensor9 = 0;
    }
    if (SensorData.Sensor10 > 60)
    {
        SensorData.Sensor10 = 0;
    }

    if (GoFoodTime > 99)
    {
        GoFoodTime = 0;
    }

    if (GoOxTime > 99)
    {
        GoOxTime = 0;
    }
}

/******************
功能：手动控制界面
参数：无
返回值：无
******************/
void Manual_Contrl(void)
{
    /*按键检测*/
    if (isKey2)
    {
        IndexFlag++;

        if (IndexFlag == 5)
        {
            IndexFlag = 1; // 选项个数限制
        }

        if (IndexFlag == 1)
        {
            CursorFlag = 1;
        }

        if (IndexFlag == 4)
        {
            CursorFlag = 0;
        }

        if (IndexFlag == 1 || IndexFlag == 4)
        {
            oled_Clear();
        }

        if (IndexFlag == 2)
        {
            CursorFlag = 2;
        }

        if (IndexFlag == 3)
        {
            CursorFlag = 3;
        }

        isKey2 = 0;
    }

    if (isKey3)
    {
        IndexFlag--;

        if (IndexFlag <= 1)
        {
            IndexFlag = 1; // 选项个数限制
        }

        if (IndexFlag == 1)
        {
            CursorFlag = 1;
        }

        if (IndexFlag == 4)
        {
            CursorFlag = 0;
        }

        if (IndexFlag == 1 || IndexFlag == 4 || IndexFlag == 3)
        {
            oled_Clear();
        }

        if (IndexFlag == 2)
        {
            CursorFlag = 2;
        }

        if (IndexFlag == 3)
        {
            CursorFlag = 3;
        }

        isKey3 = 0;
    }

    if (isKey4)
    {
        if (IndexFlag == 1)
        {
            System.Execute3 = !System.Execute3;
        }
        else if (IndexFlag == 2)
        {
            System.Execute4 = !System.Execute4;
        }
        else if (IndexFlag == 3)
        {
            System.Execute5 = !System.Execute5;
        }
        else if (IndexFlag == 4)
        {
            System.Execute7 = !System.Execute7;
        }

        isKey4 = 0;
    }

    if (IndexFlag <= 3)
    { // 第一页
        // 手动模式
        oled_ShowCHinese(16 * 2, 2 * 0, 66);
        oled_ShowCHinese(16 * 3, 2 * 0, 67);
        oled_ShowCHinese(16 * 4, 2 * 0, 68);
        oled_ShowCHinese(16 * 5, 2 * 0, 69);

        // 第一行显示加热 	判断状态开启或者关闭
        oled_ShowCHinese(16 * 0, 2 * 1, 35);
        oled_ShowCHinese(16 * 1, 2 * 1, 36);
        oled_ShowString(16 * 3, 2 * 1, ":", 16);

        if (System.Execute3 == 1)
        {
            oled_ShowCHinese(16 * 4, 2 * 1, 21);
            oled_ShowCHinese(16 * 5, 2 * 1, 22);
        }
        else
        {
            oled_ShowCHinese(16 * 4, 2 * 1, 23);
            oled_ShowCHinese(16 * 5, 2 * 1, 24);
        }

        // 第2行显示加水 判断状态开启或者关闭
        oled_ShowCHinese(16 * 0, 2 * 2, 37);
        oled_ShowCHinese(16 * 1, 2 * 2, 38);
        oled_ShowString(16 * 3, 2 * 2, ":", 16);

        if (System.Execute4 == 1)
        {
            oled_ShowCHinese(16 * 4, 2 * 2, 21);
            oled_ShowCHinese(16 * 5, 2 * 2, 22);
        }
        else
        {
            oled_ShowCHinese(16 * 4, 2 * 2, 23);
            oled_ShowCHinese(16 * 5, 2 * 2, 24);
        }

        // 第四行显示投喂 判断状态开启或者关闭
        oled_ShowCHinese(16 * 0, 2 * 3, 60);
        oled_ShowCHinese(16 * 1, 2 * 3, 61);
        oled_ShowString(16 * 3, 2 * 3, ":", 16);

        if (System.Execute5 == 1)
        {
            oled_ShowCHinese(16 * 4, 2 * 3, 21);
            oled_ShowCHinese(16 * 5, 2 * 3, 22);
        }
        else
        {
            oled_ShowCHinese(16 * 4, 2 * 3, 23);
            oled_ShowCHinese(16 * 5, 2 * 3, 24);
        }
    }
    else
    { // 第二页

        //        //增氧
        //        oled_ShowCHinese(16 * 0, 2 * 0, 13);
        //        oled_ShowCHinese(16 * 1, 2 * 0, 14);
        //        oled_ShowString(16 * 3, 2 * 0, ":", 16);

        //        if (System.Execute6 == 1) {
        //            oled_ShowCHinese(16 * 4, 2 * 0, 21);
        //            oled_ShowCHinese(16 * 5, 2 * 0, 22);
        //        } else {
        //            oled_ShowCHinese(16 * 4, 2 * 0, 23);
        //            oled_ShowCHinese(16 * 5, 2 * 0, 24);
        //        }

        // 显示蜂鸣器 判断状态开启或者关闭
        oled_ShowCHinese(16 * 0, 2 * 0, 41);
        oled_ShowCHinese(16 * 1, 2 * 0, 42);
        oled_ShowCHinese(16 * 2, 2 * 0, 43);
        oled_ShowString(16 * 3, 2 * 0, ":", 16);

        if (System.Execute7 == 1)
        {
            oled_ShowCHinese(16 * 4, 2 * 0, 21);
            oled_ShowCHinese(16 * 5, 2 * 0, 22);
        }
        else
        {
            oled_ShowCHinese(16 * 4, 2 * 0, 23);
            oled_ShowCHinese(16 * 5, 2 * 0, 24);
        }
    }

    if (CursorFlag == 0)
    {
        oled_ShowString(16 * 7, 2 * 0, "*", 16);
    }
    else
    {
        oled_ShowString(16 * 7, 2 * 0, " ", 16);
    }

    if (CursorFlag == 1)
    {
        oled_ShowString(16 * 7, 2 * 1, "*", 16);
    }
    else
    {
        oled_ShowString(16 * 7, 2 * 1, " ", 16);
    }

    // 第三行
    if (CursorFlag == 2)
    {
        oled_ShowString(16 * 7, 2 * 2, "*", 16);
    }
    else
    {
        oled_ShowString(16 * 7, 2 * 2, " ", 16);
    }

    // 第四行
    if (CursorFlag == 3)
    {
        oled_ShowString(16 * 7, 2 * 3, "*", 16);
    }
    else
    {
        oled_ShowString(16 * 7, 2 * 3, " ", 16);
    }
}

/******************
功能：阈值设置
参数：无
返回值：无
******************/
void ThresholdSet(void)
{
    // 阈值设置
    oled_ShowCHinese(16 * 2, 2 * 0, 62);
    oled_ShowCHinese(16 * 3, 2 * 0, 63);
    oled_ShowCHinese(16 * 4, 2 * 0, 64);
    oled_ShowCHinese(16 * 5, 2 * 0, 65);

    /*按键检测*/
    if (isKey2)
    {
        IndexFlag1++;

        if (IndexFlag1 == 3)
        {
            IndexFlag1 = 1;
        }

        if (IndexFlag1 == 1)
        {
            CursorFlag1 = 1;
        }
        else if (IndexFlag1 == 2)
        {
            CursorFlag1 = 2;
        }

        isKey2 = 0;
    }
    else if (isKey3)
    {
        isKey3 = 0;

        if (IndexFlag1 == 1)
        {
            Threshold.Sensor3MAX++;
        }
        else if (IndexFlag1 == 2)
        {
            Threshold.Sensor4MAX++;
        }
    }
    else if (isKey4)
    {
        isKey4 = 0;

        if (IndexFlag1 == 1)
        {
            Threshold.Sensor3MAX--;
        }
        else if (IndexFlag1 == 2)
        {
            Threshold.Sensor4MAX--;
        }
    }

    // 第三行	温度
    oled_ShowCHinese(16 * 0, 2 * 1, 8);
    oled_ShowCHinese(16 * 1, 2 * 1, 9);
    oled_ShowCHinese(16 * 2, 2 * 1, 29);
    oled_ShowCHinese(16 * 3, 2 * 1, 30);
    oled_ShowString(16 * 4, 2 * 1, ":", 16);
    oled_ShowNum(16 * 5, 2 * 1, Threshold.Sensor3MAX, 2, 16);

    oled_ShowCHinese(16 * 0, 2 * 2, 11);
    oled_ShowCHinese(16 * 1, 2 * 2, 12);
    oled_ShowCHinese(16 * 2, 2 * 2, 29);
    oled_ShowCHinese(16 * 3, 2 * 2, 30);
    oled_ShowString(16 * 4, 2 * 2, ":", 16);
    oled_ShowNum(16 * 5, 2 * 2, Threshold.Sensor4MAX, 2, 16);

    if (CursorFlag1 == 1)
    {
        oled_ShowString(16 * 7, 2 * 1, "*", 16);
    }
    else
    {
        oled_ShowString(16 * 7, 2 * 1, " ", 16);
    }

    // 第三行
    if (CursorFlag1 == 2)
    {
        oled_ShowString(16 * 7, 2 * 2, "*", 16);
    }
    else
    {
        oled_ShowString(16 * 7, 2 * 2, " ", 16);
    }

    // 第四行
    if (CursorFlag1 == 3)
    {
        oled_ShowString(16 * 7, 2 * 3, "*", 16);
    }
    else
    {
        oled_ShowString(16 * 7, 2 * 3, " ", 16);
    }

    // 阈值限幅
    if (Threshold.Sensor1MAX >= 100)
    {
        Threshold.Sensor1MAX = 100;
    }

    if (Threshold.Sensor2MAX >= 100)
    {
        Threshold.Sensor2MAX = 100;
    }

    if (Threshold.Sensor3MAX >= 100)
    {
        Threshold.Sensor3MAX = 100;
    }

    if (Threshold.Sensor4MAX >= 100)
    {
        Threshold.Sensor4MAX = 100;
    }
}
void Manual_Mode()
{
    // 换水
    if (System.Execute == 1)
    {
        Execute_ON;
    }
    else
    {
        Execute_OFF;
    }

    // 灯光
    if (System.Execute2 == 1)
    {
        Execute2_ON;
    }
    else
    {
        Execute2_OFF;
    }

    // 加热
    if (System.Execute3 == 1)
    {
        Execute3_ON;
    }
    else
    {
        Execute3_OFF;
    }

    // 加水
    if (System.Execute4 == 1)
    {
        Execute4_ON;
    }
    else
    {
        Execute4_OFF;
    }

    // 投喂
    if (System.Execute5 == 1)
    {
        Execute5_ON;
    }
    else
    {
        Execute5_OFF;
    }

    // 增氧
    if (System.Execute6 == 1)
    {
        Execute6_ON;
    }
    else
    {
        Execute6_OFF;
    }
    // 蜂鸣器
    if (System.Execute7 == 1)
    {
        Execute7_ON;
    }
    else
    {
        Execute7_OFF;
    }

    // 定时计算
    if (TimeFlag1 == 1)
    {

        if (SensorData.Sensor7 == 0)
        {
            if (SensorData.Sensor6 > 0)
            {
                SensorData.Sensor6--;
                SensorData.Sensor7 = 59;
            }

            else if (SensorData.Sensor6 == 0)
            {
                if (SensorData.Sensor5 > 0)
                {
                    SensorData.Sensor5--;
                    SensorData.Sensor6 = 60;
                }
                else
                {
                    TimeFlag1 = 0;
                    GoFood = 1;
                }
            }
        }
    }

    // 定时计算
    if (STimeFlag1 == 1)
    {

        if (SensorData.Sensor10 == 0)
        {
            if (SensorData.Sensor9 > 0)
            {
                SensorData.Sensor9--;
                SensorData.Sensor10 = 59;
            }

            else if (SensorData.Sensor9 == 0)
            {
                if (SensorData.Sensor8 > 0)
                {
                    SensorData.Sensor8--;
                    SensorData.Sensor9 = 60;
                }
                else
                {
                    STimeFlag1 = 0;
                    GoOx = 1;
                }
            }
        }
    }
}
