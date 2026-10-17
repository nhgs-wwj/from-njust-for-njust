#ifndef __SYSINIT_H
#define __SYSINIT_H


#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

void SysInit(void);
void delay_Init(void);
void delay_ms(uint32_t nms);
void delay_us(uint32_t nus);



#endif


