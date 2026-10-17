#include "led.h"
#include "sysinit.h"

volatile uint8_t led_flag=0;  /*模式标志位*/
volatile uint8_t led_delay=0; /*频率标志位*/

void GPIO_Init()
{   
    /*LED初始化*/
     MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);
     MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1);
     MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
     GPIO_setOutputLowOnPin(GPIO_PORT_P2,1);
     GPIO_setOutputLowOnPin(GPIO_PORT_P2,2);
     GPIO_setOutputLowOnPin(GPIO_PORT_P2,4);
    /*碰撞初始化*/
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P4,GPIO_PIN0|GPIO_PIN2|GPIO_PIN3|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7);
    /*编码器初始化*/
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P9,GPIO_PIN2);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P10,GPIO_PIN5);
}

void RED_Init()
{
     MAP_GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN3);
     GPIO_setOutputLowOnPin(GPIO_PORT_P5,GPIO_PIN3);
     MAP_GPIO_setAsInputPin(GPIO_PORT_P7,PIN_ALL8);

}
 
void Colour(uint8_t colour)
{
	switch (colour)
	{
		case 0://红色
				GPIO_setOutputHighOnPin(GPIO_PORT_P2,1);
				GPIO_setOutputLowOnPin(GPIO_PORT_P2,2);
				GPIO_setOutputLowOnPin(GPIO_PORT_P2,4);
				break;
		case 1://绿色
				GPIO_setOutputLowOnPin(GPIO_PORT_P2,1);
				GPIO_setOutputHighOnPin(GPIO_PORT_P2,2);
				GPIO_setOutputLowOnPin(GPIO_PORT_P2,4);
				break;
		case 2://蓝色
				GPIO_setOutputLowOnPin(GPIO_PORT_P2,1);
				GPIO_setOutputLowOnPin(GPIO_PORT_P2,2);
				GPIO_setOutputHighOnPin(GPIO_PORT_P2,4);
				break;
		case 3://黄色
				GPIO_setOutputHighOnPin(GPIO_PORT_P2,1);
				GPIO_setOutputHighOnPin(GPIO_PORT_P2,2);
				GPIO_setOutputLowOnPin(GPIO_PORT_P2,4);
				break;
		case 4://品红色
				GPIO_setOutputHighOnPin(GPIO_PORT_P2,1);
				GPIO_setOutputLowOnPin(GPIO_PORT_P2,2);
				GPIO_setOutputHighOnPin(GPIO_PORT_P2,4);
				break;
		case 5://青色
				GPIO_setOutputLowOnPin(GPIO_PORT_P2,1);
				GPIO_setOutputHighOnPin(GPIO_PORT_P2,2);
				GPIO_setOutputHighOnPin(GPIO_PORT_P2,4);
				break;
		case 6://白色
				GPIO_setOutputHighOnPin(GPIO_PORT_P2,1);
				GPIO_setOutputHighOnPin(GPIO_PORT_P2,2);
				GPIO_setOutputHighOnPin(GPIO_PORT_P2,4);
				break;
		
		default://灭
				GPIO_setOutputLowOnPin(GPIO_PORT_P2,1);
				GPIO_setOutputLowOnPin(GPIO_PORT_P2,2);
				GPIO_setOutputLowOnPin(GPIO_PORT_P2,4);
		break;
	}
	
	
	
	
	
}
void LED_Mode()
{   
    if(led_delay)
		{
			delay_ms(100*led_delay);
			Colour(led_flag);
			delay_ms(100*led_delay);
			Colour(99);
		}
		else if(!led_delay)
		{
			Colour(led_flag);
		}
}

