#include "sys.h"
 
 
/**
  * @brief  MEMS???????IIC?????
  * @param  None
  * @retval	None
  */
void MEMS_CH2O_init(void)
{
	IIC_init(); // iic????400
}
/**
  * @brief  ??MEMS???????????
  * @param  None
  * @retval ???????? 2?? 0-100ppm ??<=2
  */
uint16_t MEMS_CH2O_read(void)
{
	uint8_t recHigh = 0,recLow = 0;
	IIC_start();
	IIC_send_byte(MEMS_CH2O_wrAddr); // slave address + write cmd
	if(IIC_wait_ack()) return 0xFFFF;
	IIC_send_byte(0xA1); // read cmd
	if(IIC_wait_ack()) return 0xFFFE;
	IIC_start();
	IIC_send_byte(MEMS_CH2O_rdAddr); // slave address + read cmd
	if(IIC_wait_ack()) return 0xFFFD;
	recHigh = IIC_read_byte(1);
	recLow = IIC_read_byte(0);
	IIC_stop();
	return (uint16_t)recHigh << 8 | recLow;
}