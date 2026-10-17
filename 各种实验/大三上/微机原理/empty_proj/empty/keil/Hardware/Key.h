#ifndef __Key_H
#define __Key_H


#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define KEY_TICK_MS          10      // TA1_0_IRQHandler 每 10ms 进一次
#define KEY_DEBOUNCE_TIME    3       // 30ms 消抖
#define KEY_SHORT_PRESS_TIME 80      // < 800ms 视为短按
#define KEY_LONG_PRESS_TIME  100     // ≥ 100 * 10ms = 1s 视为长按

extern volatile uint8_t led_flag;// LED颜色标志
extern volatile uint8_t led_delay;// LED闪烁延时倍数
extern volatile uint8_t task_mode;  // 任务模式标志

extern volatile uint16_t key_p1_press_time; // P1.1按键按下时间
extern volatile uint16_t key_p4_press_time; // P1.4按键按下时间
extern volatile uint8_t key_p1_pressed;     // P1.1按键状态
extern volatile uint8_t key_p4_pressed;     // P1.4按键状态


void KEY_Init(void);
void PORT1_IRQHandler(void);  // 端口1中断处理函数

#endif 


