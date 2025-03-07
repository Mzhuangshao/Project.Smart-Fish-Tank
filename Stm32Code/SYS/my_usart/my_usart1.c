/****************重要说明**************
使用此程序需要添加

#include "stdio.h"      
#include "stdarg.h"		
#include "string.h"

这三个头文件
**********************************/

#include "sys.h"

void My_USART1(void)
{
	GPIO_InitTypeDef  GPIO_InitStrue;
	USART_InitTypeDef USART1_InitStrue;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStrue.GPIO_Pin=USART1_GPIO_PIN_TX;
	GPIO_InitStrue.GPIO_Speed=GPIO_Speed_10MHz;		
	GPIO_Init(GPIOA,&GPIO_InitStrue);
	
	GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStrue.GPIO_Pin=USART1_GPIO_PIN_RX;//PA10		
	GPIO_Init(GPIOA,&GPIO_InitStrue);
	
	USART1_InitStrue.USART_BaudRate=9600;
	USART1_InitStrue.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART1_InitStrue.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART1_InitStrue.USART_Parity=USART_Parity_No;
	USART1_InitStrue.USART_StopBits=USART_StopBits_1;
	USART1_InitStrue.USART_WordLength=USART_WordLength_8b;
	
	USART_Init(USART1,&USART1_InitStrue);	
	
	
   	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//接收中断
	  USART_Cmd(USART1,ENABLE);	

}

void USART_SendByte(USART_TypeDef* USARTx, uint16_t Data)
{
  /* Check the parameters */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_DATA(Data)); 
    
  /* Transmit Data */
  USARTx->DR = (Data & (uint16_t)0x01FF);
  while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);

}


__align(8) char USART1_TxBuff[256];  

void u1_printf(char* fmt,...) 
{  
	unsigned int i =0,length=0;
	
	va_list ap;
	va_start(ap,fmt);
	vsprintf(USART1_TxBuff,fmt,ap);
	va_end(ap);
	
	length=strlen((const char*)USART1_TxBuff);
	while(i<length)
	{
		USART_SendByte(USART1,USART1_TxBuff[i]);		
		i++;		
	}
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
	
}
//串口中断
void USART1_IRQHandler(void)
{
//    uint8_t res;
    if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)  //判断是否发生中断
        {
            USART_ClearITPendingBit(USART1,USART_IT_RXNE);  //清除当前中断标志位
//            res =USART_ReceiveData(USART1);	
////		        USART_SendData(USART1,res);//??

//					if(res=='a'){OperateMode=0;   oled_Clear();}	

//          if(res=='b'){	OperateMode=1;    oled_Clear();	}	

//					if(res=='c'){OperateMode=2;   oled_Clear(); }	
//					
//					if(res=='d'){OperateMode=3;    oled_Clear(); }	
//										
//					if(res=='e'){GoTimeFlag=1;   TimeFlag=0; }	
//   
//					if(res=='f'){GoTimeFlag=0;   TimeFlag=0; }	
//					
//				  if(res=='g'){ 
//						TimeFlag=4;						 
//					 if(GoTimeFlag==0) TimeFlag1=1;   	else  STimeFlag1=1; 
//					}

//          if(res=='h'){ 
//						  TimeFlag=4;									 
//								   if(GoTimeFlag==0) TimeFlag1=0;   	else  STimeFlag1=0;
//					}      
//					
//					
//					if(res=='i'){ 
//					 TimeFlag=1;						 
//								   if(GoTimeFlag==0)  SensorData.Sensor5--;   	else  SensorData.Sensor8--;
//					}

//           if(res=='j'){ 
//					  TimeFlag=1;									 
//								   if(GoTimeFlag==0)  SensorData.Sensor5++;   	else  SensorData.Sensor8++;  
//					} 

//          if(res=='k'){ 
//					  TimeFlag=2;						 
//								   if(GoTimeFlag==0)  SensorData.Sensor6--;   	else  SensorData.Sensor9--;
//					} 

//          if(res=='l'){ 
//					  TimeFlag=2;									 
//								   if(GoTimeFlag==0)  SensorData.Sensor6++;   	else  SensorData.Sensor9++;  
//					} 	
//      
//          if(res=='m'){ 
//					  TimeFlag=3;						 
//								   if(GoTimeFlag==0) GoFoodTime--;   	else   GoOxTime--;
//					} 
//					
//					if(res=='n'){ 
//					 TimeFlag=3;									 
//								   if(GoTimeFlag==0)  GoFoodTime++;   	else   GoOxTime++;  
//					} 
//					
////					if(res=='o'){ 
////					  	if(IndexFlag1==4){oled_Clear();}
////												
////												   IndexFlag1=1;
////										       CursorFlag1=1;
////								          Threshold.Sensor1MAX--;   
////					} 
////					
////					
////					 if(res=='p'){ 
////						if(IndexFlag1==4){oled_Clear();}
////												
////												   IndexFlag1=1;
////										       CursorFlag1=1;
////								           Threshold.Sensor1MAX++;   
////					}

////                   if(res=='q'){ 
////					 if(IndexFlag1==4){oled_Clear();}
////										
////												   IndexFlag1=2;
////										       CursorFlag1=2;
////								           Threshold.Sensor2MAX--;   
////					} 


////                  if(res=='r'){ 
////					 	 if(IndexFlag1==4){oled_Clear();}
////										 
////												   IndexFlag1=2;
////										       CursorFlag1=2;
////								           Threshold.Sensor2MAX++;   
////					}


//                   if(res=='s'){ 
//																 	   IndexFlag1=1;
//										       CursorFlag1=1;
//								           Threshold.Sensor3MAX--;   
//					} 


//                     if(res=='t'){ 
//												 	   IndexFlag1=1;
//										       CursorFlag1=1;
//								           Threshold.Sensor3MAX++;   
//					} 


//          if(res=='u'){ 
//												  		   IndexFlag1=2;
//										       CursorFlag1=2;
//									         
//								           Threshold.Sensor4MAX--;    
//					} 


//          if(res=='v'){ 
//														   IndexFlag1=2;
//										       CursorFlag1=2;
//								           Threshold.Sensor4MAX++;     
//					}

////          if(res=='w'){ 
////					   if(IndexFlag>3){  oled_Clear();} 
////									    
////								    	 CursorFlag=1;
////										   IndexFlag=1;		  
////								       System.Execute = 1;  
////					} 


////                    if(res=='x'){ 
////					  if(IndexFlag>3){  oled_Clear();} 

////										   CursorFlag=1;
////											 IndexFlag=1;			
////											 System.Execute = 0;  		
////					}  

////          if(res=='y'){ 
////					 if(IndexFlag>3){  oled_Clear();} 
////									
////									     CursorFlag=2;
////										   IndexFlag=2;		  
////								       System.Execute2 = 1;  
////					} 


////          if(res=='z'){ 
////					  if(IndexFlag>3){  oled_Clear();} 
////										 
////										  CursorFlag=2;
////											 IndexFlag=2;			
////											 System.Execute2 = 0;  		
////					} 

//          if(res=='A'){ 
//									     if(IndexFlag>3){  oled_Clear();} 
//									    
//								    	 CursorFlag=1;
//										   IndexFlag=1;		   
//								       System.Execute3 = 1;  
//					} 
//        

//          if(res=='B'){ 
//									     if(IndexFlag>3){  oled_Clear();} 
//									    
//								    	 CursorFlag=1;
//										   IndexFlag=1;		 
//											 System.Execute3 = 0;  		
//					} 					
//					
//					
//					          if(res=='C'){ 
//							 if(IndexFlag>3){  oled_Clear();} 
//									
//									     CursorFlag=2;
//										   IndexFlag=2;								
//								       System.Execute4 = 1;  
//					} 
//										
//					
//					          if(res=='D'){ 
//									 if(IndexFlag>3){  oled_Clear();} 
//									
//									     CursorFlag=2;
//										   IndexFlag=2;		
//											 System.Execute4 = 0;  					
//					} 
//										
//					
//					          if(res=='E'){ 
//											 if(IndexFlag>3){  oled_Clear();} 
//											
//											CursorFlag=3;		
//										   IndexFlag=3;		  
//								       System.Execute5 = 1;  
//					}

//          if(res=='F'){ 
//											 if(IndexFlag>3){  oled_Clear();} 
//											
//											CursorFlag=3;		
//										   IndexFlag=3;			
//											 System.Execute5 = 0;  				
//					} 


//          if(res=='G'){ 
//									  if(IndexFlag<=3){  oled_Clear();} 
//									 
//									      CursorFlag=0;		
//										   IndexFlag=4;		  
//								       System.Execute6 = 1;  
//					} 


//          if(res=='H'){ 
//										  if(IndexFlag<=3){  oled_Clear();} 
//										 
//                          CursorFlag=0;		
//										   IndexFlag=4;			
//											 System.Execute6 = 0;  	 				
//					} 	


//          if(res=='I'){ 
//										 if(IndexFlag<=3){  oled_Clear();} 
//										
//										     CursorFlag=1;		
//										   IndexFlag=5;		  
//								       System.Execute7 = 1;   
//					}


//          if(res=='J'){ 
//										  if(IndexFlag<=3){  oled_Clear();} 
//										 
//										   CursorFlag=1;		
//											 IndexFlag=5;			
//											 System.Execute7 = 0;  			
//					}



  				

					
	}
}





