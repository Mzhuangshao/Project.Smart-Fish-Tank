#ifndef __MENU_H
#define __MENU_H
#include "sys.h"

extern int16_t Servo_Speed;
extern uint8_t OperateMode;
extern uint8_t SwitchFlag1, SwitchFlag2, SwitchFlag3, SwitchFlag4;
extern uint8_t IndexFlag1;//选项索引
extern uint8_t CursorFlag1;//光标索引
extern uint8_t IndexFlag;//选项索引
extern uint8_t CursorFlag;//光标索引
void Inform_Show(void) ;
void Inform_Show2(void) ;
void Mode_Decide(void);//模式判定
void AutoContrl(void);
void Manual_Contrl(void);
void ThresholdSet(void);
void ThresholdSet1(void);
void SeasonSet(void);
void Timed_mode(void);
void Manual_Mode(void);
//增氧和投喂时间切换
extern uint8_t GoTimeFlag ;
extern uint8_t       TimeFlag1;
extern uint8_t       STimeFlag1;
extern uint8_t  TimeFlag;
//投喂 增氧时间
extern uint8_t GoFoodTime;
extern uint8_t GoOxTime;
//投喂 增氧
extern uint8_t GoFood;
extern uint8_t GoOx;
//void Threshold_Init(THRESHOLD *Threshold);
#endif

