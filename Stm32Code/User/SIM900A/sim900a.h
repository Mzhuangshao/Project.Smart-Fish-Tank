#ifndef __SIM900A_H
#define __SIM900A_H
#include "stm32f10x.h"
#include "sys.h"

#define swap16(x) (x&0XFF)<<8|(x&0XFF00)>>8		//高低字节交换宏定义
void sim900a_unigbk_exchange(u8 *src,u8 *dst,u8 mode);
u8 sim900a_hex2chr(u8 hex);
u8 sim900a_chr2hex(u8 chr);
u8 sim900a_send_cmd(u8 *cmd,u8 *ack,u16 waittime);
u8* sim900a_check_cmd(u8 *str);
void sim_at_response(u8 mode);
void sim900a_send_English_message(char *message,char *phonenumber);
u8 Find_char(char *a,char *b);  //b为子串
void sim900a_Call(char *phonenumber);
#endif

