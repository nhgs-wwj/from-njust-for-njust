#include "TIM.h"
#include "sysinit.h"
#include "led.h"
#include "Key.h"
#include <stdint.h>
#include "route.h"
#include "pid.h"
#include "bump.h"
#include "PWM.h"
#include "IR_Sensor.h"

//目标中心速
float spe_c = 0;
//差速比例
float gap_rate = 0;
//碰撞延时系数（正比转弯时间）
float BZ_ratio = 3;
//碰撞回退差速（正比转弯半径）
float BZ_gap = 0.5;
//碰撞回退速度
float BZ_spe = 600;
//碰撞回退时间
uint16_t BZ_delay = 0;
	//巡线
PID_TypeDef line_pid = {
	.kp = 0.20,//0.25
	.ki = 0,
	.kd = 0.8,//0.8
	.target = 0,
	.out_lmt_en = 1,
	.max_out = +0.9,
	.min_out = -0.9
};

extern volatile uint8_t led_flag;  /*模式标志位*/
extern volatile uint8_t led_delay; /*频率标志位*/
extern volatile uint8_t task_mode;

extern uint8_t speed_flag;
extern uint8_t dir_flag;

uint8_t BZ_mode = 0;

/*周期为10ms*/
void TIM_A1_Init() 
{  
    // 配置定时器A1为向上计数模式
    Timer_A_UpModeConfig upConfig = {
        .clockSource = TIMER_A_CLOCKSOURCE_SMCLK,           // SMCLK时钟源
        .clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_48, // 48分频
        .timerPeriod = 10000 - 1,                            // 10ms中断周期
        .timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE,     // 禁用定时器溢出中断
        .captureCompareInterruptEnable_CCR0_CCIE = TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE, // 启用CCR0中断
        .timerClear = TIMER_A_DO_CLEAR                      // 清零计数器
    };
    
    
		// 初始化定时器A2
    MAP_Timer_A_configureUpMode(TIMER_A1_BASE, &upConfig);

    
    // 使能定时器A1 CCR0中断
    MAP_Interrupt_enableInterrupt(INT_TA1_0);
    
    // 启动定时器
    MAP_Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);
}

/*周期为5ms*/
void TIM_A2_Init(void)
{
    // 1. 配置定时器A2为向上计数模式
    Timer_A_UpModeConfig upConfig = {
        .clockSource = TIMER_A_CLOCKSOURCE_SMCLK,           // SMCLK时钟源
        .clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_48, // 48分频
        .timerPeriod = 5000 - 1,                            // 5ms中断周期
        .timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE,     // 禁用定时器溢出中断
        .captureCompareInterruptEnable_CCR0_CCIE = TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE, // 启用CCR0中断
        .timerClear = TIMER_A_DO_CLEAR                      // 清零计数器
    };
    
    // 2. 初始化定时器A2
    MAP_Timer_A_configureUpMode(TIMER_A2_BASE, &upConfig);
    
    // 3. 启动计数器
    MAP_Timer_A_startCounter(TIMER_A2_BASE, TIMER_A_UP_MODE);
    
    // 4. 使能定时器A2 CCR0中断
    MAP_Interrupt_enableInterrupt(INT_TA2_0);
}


/*TIMA0*/
void TIM_PWM_Init()
{

        /*复用为PWM输出*/
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN6, GPIO_PRIMARY_MODULE_FUNCTION); //通道3  R
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION); //通道4  L

    // PWM配置 - 通道3（右电机）
    Timer_A_PWMConfig pwmConfig3 = {
        TIMER_A_CLOCKSOURCE_SMCLK,
        48,                                // 分频48
        1000-1,                           // 周期1000
        TIMER_A_CAPTURECOMPARE_REGISTER_3,//通道
        TIMER_A_OUTPUTMODE_RESET_SET,     // 标准PWM模式
        0                                 // 初始占空比0%
    };
    MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig3);

    // PWM配置 - 通道4（左电机）
    Timer_A_PWMConfig pwmConfig4 = {
        TIMER_A_CLOCKSOURCE_SMCLK,
        48,
        1000-1,
        TIMER_A_CAPTURECOMPARE_REGISTER_4, 
        TIMER_A_OUTPUTMODE_RESET_SET,
        0
    };
    MAP_Timer_A_generatePWM(TIMER_A0_BASE, &pwmConfig4);                /* 初始化比较寄存器以产生 PWM4 */

}

void TA1_0_IRQHandler()
{
	 // 清除中断标志
		Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_0);
    //ADC读取红外数据
	  MAP_ADC14_toggleConversionTrigger();

    
    // 按键检测处理
    // 检测P1.1按键
    if(key_p1_pressed)
		{
        if(GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1) == 1) 
				{
            // 按键已释放
            if(key_p1_press_time >= KEY_DEBOUNCE_TIME && key_p1_press_time < KEY_SHORT_PRESS_TIME) 
						{
                // 短按处理
							switch (task_mode)
							{
								case 1:
									led_flag = (led_flag + 1) % 7;
								break;
								
//								case 2:
//									speed_flag = (speed_flag + 1)%4;
//								break;
								
							}

            }
						key_p1_pressed = 0;
            key_p1_press_time = 0;
        } 
				else 
				{
            // 按键仍处于按下状态
						if (key_p1_press_time < 0xFFFF)
              key_p1_press_time++;
            // 长按处理
            if(key_p1_press_time == KEY_LONG_PRESS_TIME) 
						{ 
							
                key_p1_press_time = KEY_LONG_PRESS_TIME + 1; // 防止重复触发
            }
        }
    }
    
    // 检测P1.4按键
    if(key_p4_pressed) 
		{
        if(GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4) == 1) 
				{
						// 按键已释放
						if(key_p4_press_time >= KEY_DEBOUNCE_TIME && 
               key_p4_press_time < KEY_SHORT_PRESS_TIME) 
						{
							// 短按处理
							switch (task_mode)
							{
								case 1:
									led_delay = (led_delay + 1) % 4;
								break;
								
//								case 2:
//									dir_flag = (dir_flag + 1)%4;
//								break;
								
								case 3:
									BZ_mode = (BZ_mode + 1)%2;
								break;
							}


						}
						key_p4_pressed = 0;
            key_p4_press_time = 0;
        } 
				else					
				{
            // 按键仍处于按下状态
						if (key_p4_press_time < 0xFFFF) 
               key_p4_press_time++;
            // 长按处理
            if(key_p4_press_time == KEY_LONG_PRESS_TIME) 
						{
                task_mode =(task_mode + 1)%4;
                key_p4_press_time = KEY_LONG_PRESS_TIME + 1; // 轻微超门限，避免再次 == 命中
            }
        }
    }

}

void TA2_0_IRQHandler(void)
{
	static uint8_t count = 0;
	Timer_A_clearCaptureCompareInterrupt(TIMER_A2_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_0);
	
	if(count >= 2)
			count = 0;
	else 
			count += 1;
	
	switch(count)
	{
		case(0):{
			if(task_mode == 2)
			{
				//巡线环
				Get_Route_Update();
				line_pid.value = ROUTE_ERR;
				gap_rate = -pid_realize_lo(&line_pid);
				spe_c = base_compare;
			}
		}break;
		
		case(1):{
			if(task_mode == 3 && BZ_mode == 0)
			{
				//红外避障
				IR_GetDistances(distanceBuffer);
				Get_Distance_Update();
				float ABS_ERR = (IR_ERR<0)?(-IR_ERR):(IR_ERR);
				if(IR_VALID) 
				{
					BZ_delay = ABS_ERR*BZ_ratio;
					spe_c = -BZ_spe;
					gap_rate = (IR_ERR>0)?(BZ_gap):(-BZ_gap);
				}
				else if(BZ_delay)
				{
					BZ_delay --;
				}
				else
				{
					spe_c = BZ_spe;
					gap_rate = 0;
				}

			}
			if(task_mode == 3 && BZ_mode == 1)
			{
				//碰撞避障
				Bump_Read();
				Get_Bump_Update();
				float ABS_ERR = (BUMP_ERR<0)?(-BUMP_ERR):(BUMP_ERR);
				if(BUMP_VALID) 
				{
					BZ_delay = ABS_ERR*BZ_ratio;
					spe_c = -BZ_spe;
					gap_rate = (BUMP_ERR<0)?(BZ_gap):(-BZ_gap);
				}
				else if(BZ_delay)
				{
					BZ_delay --;
				}
				else
				{
					spe_c = BZ_spe;
					gap_rate = 0;
				}

			}
		}break;
		
		default:{
		}break;

	}
	
	motor_control(spe_c*(1+gap_rate),spe_c*(1-gap_rate));
}

