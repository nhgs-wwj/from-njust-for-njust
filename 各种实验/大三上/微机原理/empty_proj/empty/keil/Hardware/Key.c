#include "Key.h"
#include "sysinit.h"

#define KEY_TICK_MS          10      // TA1_0_IRQHandler 每 10ms 进一次
#define KEY_DEBOUNCE_TIME    3       // 30ms 消抖
#define KEY_SHORT_PRESS_TIME 80      // < 800ms 视为短按
#define KEY_LONG_PRESS_TIME  100     // ≥ 100 * 10ms = 1s 视为长按

extern volatile uint8_t led_flag;
extern volatile uint8_t led_delay;
extern volatile uint8_t task_mode;

volatile uint16_t key_p1_press_time = 0;
volatile uint16_t key_p4_press_time = 0;
volatile uint8_t key_p1_pressed = 0;
volatile uint8_t key_p4_pressed = 0;

void KEY_Init()
{   
    /*上拉*/
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1,GPIO_PIN1|GPIO_PIN4);
    
    /*配置外部中断*/
    
    GPIO_clearInterruptFlag(GPIO_PORT_P1,GPIO_PIN1|GPIO_PIN4);
    GPIO_interruptEdgeSelect(GPIO_PORT_P1,GPIO_PIN1|GPIO_PIN4,GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_enableInterrupt(GPIO_PORT_P1,GPIO_PIN1|GPIO_PIN4);
    
    /*开启端口中断*/
    
    Interrupt_enableInterrupt(INT_PORT1);
    
}


void PORT1_IRQHandler()
{
    uint16_t status;
    status=GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);//获取已经使能且触发中断的引脚状态
    GPIO_clearInterruptFlag(GPIO_PORT_P1,status);//清除中断标志，否则会连续进入中断
    if(status&GPIO_PIN1)
    {
        if(GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1) == 0) 
				{
            // P1.1按键按下，设置按下标志
            key_p1_pressed = 1;//设置按键按下标志
            key_p1_press_time = 0;//重置按下时间计数器
        }

    }
    else if(status&GPIO_PIN4)
    {
       if(GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4) == 0) 
				{
            // P1.4按键按下，设置按下标志
            key_p4_pressed = 1;
            key_p4_press_time = 0;//开始计时，用于检测长按
        }
     
    }
}
