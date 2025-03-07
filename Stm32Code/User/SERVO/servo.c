#include "sys.h"

void SG90_Init(void)
{
	GPIO_InitTypeDef temp;
	RCC_APB2PeriphClockCmd(SG90_GPIO_CLK,ENABLE);
	temp.GPIO_Mode=GPIO_Mode_Out_PP;
	temp.GPIO_Pin=SG90_GPIO_PIN;
	temp.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(SG90_GPIO_PORT,&temp);

}
/*传入参数对应转动多少角度   500us对应0度

											500us,0�
											1000us,45�
											1500us,90�
											2000us,145�
											2500us,180�
					  
*/
void Control_SG90(uint32_t us)
{
		
	int i=0;
	for(i=0;i<10;i++)
	{
		if(us<=20000)
		{
			PBout(15)=1;
			delay_us(us);
			PBout(15)=0;
			delay_us(20000-us);
		}
	}

}
