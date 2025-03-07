#ifndef __MEMSS_H
#define __MEMSS_H
 
#include "sys.h"
 
#define MEMS_CH2O_wrAddr	0x54
#define MEMS_CH2O_rdAddr	0x55
 
void MEMS_CH2O_init(void);
uint16_t MEMS_CH2O_read(void);
 
#endif