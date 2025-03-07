
#include "sys.h"
void DHT22_Rst(void)	   
{
	DHT22_IO_OUT();//SET OUTPUT
	DHT22_DQ_OUT=0;//??DQ
	delay_ms(1);//??????500us
	DHT22_DQ_OUT=1;//DQ=1
	delay_us(30);//????20~40us
}

u8 DHT22_Check(void) 	   
{   
	u8 retry=0;
	DHT22_IO_IN();//SET INPUT	 
    while (DHT22_DQ_IN&&retry<200)//?????
	{
		retry++;
		delay_us(1);
	}	 
	if(retry>=200)return 1;//??retry??200????1
	else retry=0;
    while (!DHT22_DQ_IN&&retry<200)//DHT22????????80us??
	{
		retry++;
		delay_us(1);
	}
	if(retry>=200)return 1;	    
	else return 0;
}
//?DHT22?????
//???:1/0
u8 DHT22_Read_Bit(void) 			 
{
 	u8 retry=0;
	while(DHT22_DQ_IN&&retry<200)//???????
	{
		retry++;
		delay_us(1);
	}
	retry=0;
	while(!DHT22_DQ_IN&&retry<200)//??????
	{
		retry++;
		delay_us(1);
	}
	delay_us(40);//??40us
	if(DHT22_DQ_IN)return 1;
	return 0;		   
}

u8 DHT22_Read_Byte(void)    
{        
    u8 i,dat;
    dat=0;
	for (i=0;i<8;i++) 
	{
   		dat<<=1; 
	    dat|=DHT22_Read_Bit();
    }						    
    return dat;
}

u8 DHT22_Read_Data(u16 *temp,u16 *humi)    
{        
 	u8 buf[5];
	u8 i,sum;
	DHT22_Rst();//????
	if(DHT22_Check()==0)
	{
		for(i=0;i<5;i++)//??40???
		{
			buf[i]=DHT22_Read_Byte();
		}
//		*humi=buf[0]*256+buf[1];
//		*temp=buf[2]*256+buf[3];
		sum =((buf[0]+buf[1]+buf[2]+buf[3])&0xFF);
		if(sum==buf[4])//??
		{
            
			*humi=buf[0]*256+buf[1];
			*temp=buf[2]*256+buf[3];
		}
	}else return 1;
	return 0;	    
}
  	 
u8 DHT22_Init(void)
{	 
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //??PE????
	
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //PE4????
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //????
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);				 //???IO?
 	GPIO_SetBits(GPIOA,GPIO_Pin_5);						 //PE4 ???
			    
	DHT22_Rst();  //??DHT22
	return DHT22_Check();//??DHT22???
} 
//dome
//u16 temp;
//u16 hui;
//  DHT22_Init();
//DHT22_Read_Data(&temp,&hui);
//hui=hui/10;	