#ifndef __TIM_H
#define __TIM_H

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

extern float spe_c;
//差速比例
extern float gap_rate;

extern volatile uint8_t led_flag;  /*模式标志位*/
extern volatile uint8_t led_delay; /*频率标志位*/
extern volatile uint8_t task_mode;

extern uint8_t speed_flag;
extern uint8_t dir_flag;
extern uint8_t BZ_mode;

void TIM_A1_Init(void) ;
void TIM_A2_Init(void) ;
void TIM_PWM_Init(void);

void TIM_Encoder_Init(void);


#endif

