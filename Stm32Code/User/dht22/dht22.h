#ifndef __DHT22_H
#define __DHT22_H 
#include "sys.h"   

#define DHT22_IO_IN()  {GPIOA->CRL&=0xFF0FFFFF;GPIOA->CRL|=8<<20;}
#define DHT22_IO_OUT() {GPIOA->CRL&=0xFF0FFFFF;GPIOA->CRL|=3<<20;}
										   
#define	DHT22_DQ_OUT PAout(5) //????	PE4
#define	DHT22_DQ_IN  PAin(5)  //????	PE4
 
 
u8 DHT22_Init(void);//???DHT22
u8 DHT22_Read_Data(u16 *temp,u16 *humi);//?????
u8 DHT22_Read_Byte(void);//??????
u8 DHT22_Read_Bit(void);//?????
u8 DHT22_Check(void);//??????DHT22
void DHT22_Rst(void);//??DHT22    
#endif