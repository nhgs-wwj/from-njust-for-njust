#ifndef __PWM_H
#define __PWM_H


#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

void PWM_Init(void);    
void motor_control(int motorL,int motorR);


#endif 

