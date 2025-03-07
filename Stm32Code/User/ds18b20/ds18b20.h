#ifndef _ds18b20_H
#define _ds18b20_H
 
#include "sys.h"
 
 

#define DS18B20_PORT 			  GPIOA  
#define DS18B20_PIN 			( GPIO_Pin_5)
#define DS18B20_PORT_RCC		RCC_APB2Periph_GPIOA
 
 
								   
#define	DS18B20_DQ_OUT PAout(5)
#define	DS18B20_DQ_IN  PAin(5)  
   	
		
		
u8 DS18B20_Init(void);			
float DS18B20_GetTemperture(void);	
void DS18B20_Start(void);	
void DS18B20_Write_Byte(u8 dat);
u8 DS18B20_Read_Byte(void);		
u8 DS18B20_Read_Bit(void);		
u8 DS18B20_Check(void);			
void DS18B20_Reset(void);			



#endif

