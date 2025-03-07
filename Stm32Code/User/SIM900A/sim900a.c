#include "sim900a.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usart.h"
#include "delay.h"
#include "usart3.h"


void sim_at_response(u8 mode)
{
	if(USART3_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//添加结束符
		printf("%s",USART3_RX_BUF);	//发送到串口
		if(mode)USART3_RX_STA=0;
	} 
}

//sim900a发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果
//    其他,期待应答结果的位置(str的位置)
u8* sim900a_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART3_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//添加结束符
		strx=strstr((const char*)USART3_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}

//向sim900a发送命令
//cmd:发送的命令字符串(不需要添加回车了),当cmd<0XFF的时候,发送数字(比如发送0X1A),大于的时候发送字符串.
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)
//       1,发送失败
u8 sim900a_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART3_RX_STA=0;
	if((u32)cmd<=0XFF)
	{
		while((USART3->SR&0X40)==0);//等待上一次数据发送完成  
		USART3->DR=(u32)cmd;
	}else u3_printf("%s\r\n",cmd);//发送命令
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART3_RX_STA&0X8000)//接收到期待的应答结果
			{
				if(sim900a_check_cmd(ack))break;//得到有效数据 
				USART3_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
}

//将1个字符转换为16进制数字
//chr:字符,0~9/A~F/a~F
//返回值:chr对应的16进制数值
u8 sim900a_chr2hex(u8 chr)
{
	if(chr>='0'&&chr<='9')return chr-'0';
	if(chr>='A'&&chr<='F')return (chr-'A'+10);
	if(chr>='a'&&chr<='f')return (chr-'a'+10); 
	return 0;
}
//将1个16进制数字转换为字符
//hex:16进制数字,0~15;
//返回值:字符
u8 sim900a_hex2chr(u8 hex)
{
	if(hex<=9)return hex+'0';
	if(hex>=10&&hex<=15)return (hex-10+'A'); 
	return '0';
}

/**********************************************************************
描述: ASCII 转 unicode      比如 '1'  转成 "0031"
***********************************************************************/
void ASCII_TO_Unicode(char *ASCII,char *Unicode)
{
    int length;
    int i = 0;
    int j = 0;
	memset(Unicode,'\0',sizeof(Unicode));
    length = strlen(ASCII);

    for(i=0;i<length;i++)
    {
        Unicode[j++] = '0';
        Unicode[j++] = '0';

        Unicode[j++] = (ASCII[i] / 16) + 0x30;
        Unicode[j++] = (ASCII[i] % 16) + 0x30;
    }

}




char dispbuf[64];
extern u8 Usart3_buff[128];     /*static和extern的区别*/

/**************************************************************************/
//函数作用：字符串匹对函数
//函数名称：Find_char(char *a,char *b) ;
//内部参数：数组a   数组b
/**************************************************************************/
u8 Find_char(char *a,char *b)  //b为子串
{ 
  if(strstr(a,b)!=NULL)
	    return 0;
	else
			return 1;
}

/**************************************************************************/
//函数作用：发送英文短信函数
//函数名称：sim900a_send_English_message(char *message,char *phonenumber)(uint8_t number);
//内部参数：message phonenumber
/**************************************************************************/
void sim900a_send_English_message(char *message,char *phonenumber)
{
	Usart_SendString(USART3,"AT\r\n");                            //SIM900A是否与单片机来连接成功
	delay_ms(200);	
	while(Find_char((char*)Usart3_buff,"OK"));                      
	printf("%s\r\n",Usart3_buff);
//    memset(Usart3_buff,0x00,sizeof(Usart3_buff));
	
	Usart_SendString(USART3,"AT&F\r\n");                           //SIM900A复位
	delay_ms(200);	
	while(Find_char((char*)Usart3_buff,"OK"));                     //字符串匹对函数   
//	printf("%s\r\n",Usart3_buff);
	
    
	Usart_SendString(USART3,"AT+CSCS=\"GSM\"\r\n");               //英文短信指令1
	delay_ms(200);	
	while(Find_char((char*)Usart3_buff,"OK"));                     
//	printf("%s\r\n",Usart3_buff);
	
	
	Usart_SendString(USART3,"AT+CMGF=1\r\n");                     //英文短信指令2
	delay_ms(200);	
	while(Find_char((char*)Usart3_buff,"OK"));  
//	printf("%s\r\n",Usart3_buff);
	
	sprintf(dispbuf,"AT+CMGS=\"%s\"\r\n",phonenumber);
	Usart_SendString(USART3,dispbuf);                             //英文短信指令3
	delay_ms(200);
	while(Find_char((char*)Usart3_buff,"OK")); 
//	printf("%s\r\n",Usart3_buff);
	
	Usart_SendString(USART3,message);                              //英文短信指令4
	delay_ms(200);
	while(Find_char((char*)Usart3_buff,"OK"));  
//	printf("%s\r\n",Usart3_buff);
//	memset(Usart3_buff,0x00,sizeof(Usart3_buff));
    
	Usart_SendHalfWord(USART3,0x1a);                                //结束指令
	delay_ms(2000);  //延时两秒
	while(Find_char((char*)Usart3_buff,"OK"));    
	printf("Usart3_buff=%s\r\n",Usart3_buff);
    
} 


void sim900a_Call(char *phonenumber)
{
    Usart_SendString(USART3,"AT\r\n");                            //SIM900A是否与单片机来连接成功
	delay_ms(200);
//    printf("AT \r\n");    
	while(Find_char((char*)Usart3_buff,"OK"));                      
//	printf("AT OK\r\n");
    
    Usart_SendString(USART3,"AT+CSQ\r\n");                           //查询信号强度指令
	delay_ms(200);	
	while(Find_char((char*)Usart3_buff,"OK"));                     //字符串匹对函数   
//	printf("AT+CSQ OK\r\n");
    
    Usart_SendString(USART3,"AT+CPIN?\r\n");                           //是否检测到手机卡
	delay_ms(200);	
	while(Find_char((char*)Usart3_buff,"OK"));                     //字符串匹对函数   
//	printf("AT+CPIN? OK\r\n");
    
    Usart_SendString(USART3,"AT+COPS?\r\n");                           //是否注册到网络
	delay_ms(200);	
	while(Find_char((char*)Usart3_buff,"OK"));                     //字符串匹对函数   
//	printf("AT+COPS? OK\r\n");
    
    sprintf(dispbuf,"ATD%s;\r\n",phonenumber);
	Usart_SendString(USART3,dispbuf);                             //英文短信指令3
	delay_ms(200);
    
	while(Find_char((char*)Usart3_buff,"OK")); 
//	printf("拨号成功\r\n");
//    printf("Usart3_buff=%s\r\n",Usart3_buff);
//    delay_ms(2000);
//    memset(Usart3_buff,0x00,sizeof(Usart3_buff));
}


