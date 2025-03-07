#ifndef __GY30_H
#define __GY30_H
#include "sys.h"

#define				BH1750_Addr_GND_REG				0X23	//ADDR??????
#define				BH1750_Addr_VCC_REG				0X5C	//ADDR??????


				/******	??????*****/
#define				BH1750_Power_OFF_REG			0x00	//????
#define				BH1750_Power_ON_REG				0x01	//??,??????
#define				BH1750_MODULE_RESET_REG			0x07	//???????,?????,????????

#define				BH1750_CONTINUE_H_MODE			0x10	//??H?????
#define				BH1750_CONTINUE_H_MODE2			0x11	//??H?????2
#define				BH1750_CONTINUE_L_MODE			0x13	//??L?????
#define				BH1750_ONE_TIME_H_MODE			0x20	//??H?????
#define				BH1750_ONE_TIME_H_MODE2			0x21	//??H?????2
#define				BH1750_ONE_TIME_L_MODE			0x23	//???L?????



/**		?????	**/

void 		BH1750_WriteByte				(uint8_t addr);
uint16_t 	BH1750_Read_Data				(void);
void 		BH1750_Power_ON					(void);
void 		BH1750_Power_OFF				(void);
void 		BH1750_RESET					(void);
void 		BH1750_Init						(void);
float 	Light_Intensity					(void);
		//dome
//		BH1750_Init();
//		oled_ShowNum(16*0,2*0,Light_Intensity(),5,16);
//单位lux
	
#endif

