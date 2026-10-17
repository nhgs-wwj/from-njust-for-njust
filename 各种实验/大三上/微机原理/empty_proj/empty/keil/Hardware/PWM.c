#include "PWM.h"
#include "TIM.h"
#include "sysinit.h"

void PWM_Init()    
{   
     MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN6);   /*R DIR*/
     MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN7);   /*L DIR*/
     MAP_GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN6);   /*R SLEEP 0有效*/
     MAP_GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN7);   /*L SLEEP 0有效*/

     GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN6);
     GPIO_setOutputLowOnPin(GPIO_PORT_P3,GPIO_PIN7);

}

void motor_control(int motorL,int motorR)
{
    if(motorL>0)
    {    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN7);
    }
    else 
    {
         GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN7);
        motorL=-motorL;
    }
    if(motorR>0)
    {
        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN6);
    }
    else
    {
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN6);
        motorR=-motorR;
    }
    GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN6);
    GPIO_setOutputHighOnPin(GPIO_PORT_P3,GPIO_PIN7);
    Timer_A_setCompareValue(TIMER_A0_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_4,motorL);
    Timer_A_setCompareValue(TIMER_A0_BASE,TIMER_A_CAPTURECOMPARE_REGISTER_3,motorR);
}

