
#include "sys.h"



#define SG90_GPIO_PIN   GPIO_Pin_15
#define SG90_GPIO_PORT  GPIOB
#define SG90_GPIO_CLK  RCC_APB2Periph_GPIOB



#define Execute5_ON      			  Control_SG90(2000);   
#define Execute5_OFF      			Control_SG90(500);

void SG90_Init(void);
void Control_SG90(uint32_t us);


