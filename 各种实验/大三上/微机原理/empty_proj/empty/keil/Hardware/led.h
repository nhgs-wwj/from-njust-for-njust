#ifndef __led_H
#define __led_H


#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

void LED_Mode(void);
void GPIO_Init(void);
void RED_Init(void);
void Colour(uint8_t colour);

extern volatile uint8_t led_flag;
extern volatile uint8_t led_delay;

#endif 


