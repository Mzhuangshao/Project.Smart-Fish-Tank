#ifndef __GY30IIC_H
#define __GY30IIC_H
#include "sys.h" 


/*********************************
**	????:					**
**			SCL ---> PXx		**
**			SDA ---> PXx		**
**********************************/
	
/*******************************????***********************************/  
#define  gyIIC_Delay()		{	delay_us(5);	}	   		   

/*************************** IO????(GPIOx * 4) ****************************/
 
					/**		PXx ??????		**/
#define  gySDA_IN()  {GPIOB -> CRL &= 0X0FFFFFFF; GPIOB -> CRL |= (u32)8 << 28;}
					/**		PXx ??????		**/
#define  gySDA_OUT() {GPIOB -> CRL &= 0X0FFFFFFF; GPIOB -> CRL |= (u32)3 << 28;}





/*************************** IO???? *************************/	 
#define 		gyIIC_SCL    		PBout(6) 	//SCL ??
#define 		gyIIC_SDA    		PBout(7) 	//SDA ??	
#define 		gyRead_SDA   		PBin (7)  	//SDA ??





/***************************** IIC?????? **********************************/

void 				gyIIC_Init				(void);             //???IIC?IO?
void 				gyIIC_Start				(void);				//??IIC????
void 				gyIIC_Stop				(void);	  			//??IIC????
void 				gyIIC_Send_Byte			(uint8_t Txd);		//IIC??????
uint8_t 			gyIIC_Read_Byte			(uint8_t ACK);		//IIC??????
uint8_t 			gyIIC_Wait_Ack			(void); 			//IIC??ACK??
void 				gyIIC_Ack					(void);				//IIC??ACK??
void 				gyIIC_NAck				(void);				//IIC???ACK??


 
#endif
