/******************************************************************************
 * @Name: MSP432 Empty Project
 *
 * @Description: An empty project that uses DriverLib 
 *							 Secondary development is used for microcomputer experiments
 *               This is for reference only in the microprocessor experiment
 *          
 * @Author: nhgs_wwj , Chen , Gao and my other teammates
 * 
 * Thanks for the following references:
 *		https://gitee.com/chenjjian/MSP432_Quick_entry
 *		xiuluoliulei123
 *		fytoyu
 *		
 *	//Please do not distribute without permission. 
 *  //Future open-source release on GitHub is planned.
 *  This project has now been made open source on GitHub:https://github.com/nhgs-wwj/from-njust-for-njust. 
 *
*******************************************************************************/
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include "sysinit.h"
#include "led.h"
#include "Key.h"
#include "TIM.h"
#include "PWM.h"
#include "IR_Sensor.h"
#include "Bump.h"
#include "route.h"
#include "pid.h"

volatile uint8_t task_mode = 1;

int compare;
int compare_l;
int compare_r;

uint8_t speed_flag = 0;
uint8_t dir_flag = 0;

uint8_t test_data = 0;
void Moter_Simple_Mode(void);

int main(void)
{
		TIM_A1_Init();
		TIM_A2_Init();
		TIM_PWM_Init();
	  SysInit();
		delay_Init();
		GPIO_Init();
		PWM_Init();
		KEY_Init();
	  IR_Sensor_Init();
		Bump_Init();
		Route_Init();
		

    while(1)
    {
			delay_ms(1);
			Route_Read();
			switch (task_mode)
			{
				case 0:////test
					Colour(99);
					motor_control(0,0);
					break;
				
				case 1://LED
					LED_Mode();
					break;
				
				case 2://循迹
					Colour(6);
				break;
				
				case 3://避障
					Colour(5);
				break;
				
				
				default:
					break;
				
			}
    }
}

void Moter_Simple_Mode(void)//任务2验收用
{   

    //控制速度
    switch(speed_flag)
    {    case(1):
            compare=400;
        break;
        case(2):
            compare=800;
        break;
        case(3):
            compare=1000;
        break;
        case(0):
            compare=0;
        break;
    }   
        
    //控制方向
    switch(dir_flag)
    {
        case(1):
            compare_l=compare;
            compare_r=-compare;  //左正右负
        break;
        case(2):
            compare_l=-compare;
            compare_r=compare;    //左负右正
        break;
        case(3):
            compare_l=-compare;
            compare_r=-compare;
        break;
        case(0):
            compare_l=compare;
            compare_r=compare;
        break;    
    }    
    
    motor_control(compare_l,compare_r);
    
}
