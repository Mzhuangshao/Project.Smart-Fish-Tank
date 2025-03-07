#include "sys.h"



/****************************************************************************
**	????:BH1750_WriteByte()
**	????:BH1750 ???
**	????:
**			uint8_t addr: ?????
**
**	???:?
**	IIC????:
**				I2C_GetFlagStatus(IIC_x, IIC_Flag_x)-- ???????? 1
**
*****************************************************************************/
void BH1750_WriteByte(uint8_t addr)
{	
	gyIIC_Start();								// 1.??????
	
	gyIIC_Send_Byte(BH1750_Addr_GND_REG << 1|0);	// 2.??7???????(0:?)
	while(!gyIIC_Wait_Ack());						// 3.????????
	
	gyIIC_Send_Byte(addr);						// 4.??8?????
	while(!gyIIC_Wait_Ack());						// 5.????????
	
	gyIIC_Stop();									// 6.??????
}



/****************************************************************************
**	????:BH1750_Read_Data()
**	????:BH1750 ????
**	????:
**			uint8_t addr :?????
**
**	???:RX_Data
**	IIC????:
**				I2C_GetFlagStatus(IIC_x, IIC_Flag_x)-- ???????? 1
**
*****************************************************************************/
uint16_t BH1750_Read_Data(void)
{
    uint8_t 	H_Data;							//? 8 ???
	uint8_t 	L_Data;							//? 8 ? ??
	uint16_t 	Rx_Data;						//?? 16 ???
	
	gyIIC_Start();								// 1.??????
	gyIIC_Send_Byte(BH1750_Addr_GND_REG << 1|1);	// 2.??7???????(1:?)
	while(!gyIIC_Wait_Ack());						// 3.????????
	
	H_Data = gyIIC_Read_Byte(1);					// 4.????????ACK
	L_Data = gyIIC_Read_Byte(0);					// 5.????????NACK
	
	gyIIC_Stop();									// 6.??????
    Rx_Data = ((uint16_t)H_Data << 8) + L_Data;	// 7.16?????
	
    return Rx_Data;								// 8.??????
}


/**	BH1750?? **/
void BH1750_Power_ON(void)
{
	BH1750_WriteByte(BH1750_Power_ON_REG);
}

/**	BH1750?? **/
void BH1750_Power_OFF(void)
{
	BH1750_WriteByte(BH1750_Power_OFF_REG);
}

/**	BH1750??????? **/
void BH1750_RESET(void)
{
	BH1750_WriteByte(BH1750_MODULE_RESET_REG);
}

/**	BH1750??? **/
void BH1750_Init(void)
{
	gyIIC_Init();
	
    delay_ms(100);
	BH1750_Power_OFF();
	
	BH1750_Power_ON();
	
	BH1750_RESET();
	BH1750_WriteByte(BH1750_CONTINUE_H_MODE2);
}

/**	
**	??????
**	???	????(??lx)=(High Byte + Low Byte)/ 1.2  
**/
float Light_Intensity(void)
{
	return (float)(BH1750_Read_Data() / 1.2f );
}

/*************************************END***********************************************/
