#include "bump.h"
#include "sysinit.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define BUMP_PORT GPIO_PORT_P4
#define BUMP_PINS (GPIO_PIN7|GPIO_PIN6|GPIO_PIN5|GPIO_PIN3|GPIO_PIN2|GPIO_PIN0)

uint8_t BUMP_DATA = 0;
uint8_t BUMP_VALID = 0;
float BUMP_ERR = 0;

void Bump_Init(void)
{
    // 设置为输入 + 上拉
    MAP_GPIO_setAsInputPinWithPullUpResistor(BUMP_PORT, BUMP_PINS);

    // 禁止中断
    MAP_GPIO_disableInterrupt(BUMP_PORT, BUMP_PINS);
}

uint8_t Bump_Read(void)
{
    uint8_t result = 0;
    
    // 逐个读取每个引脚，碰撞时为1，未碰撞为0
    if (MAP_GPIO_getInputPinValue(BUMP_PORT, GPIO_PIN0) == GPIO_INPUT_PIN_LOW)
        result |= (1 << 0);  // 位1: P4.0
    if (MAP_GPIO_getInputPinValue(BUMP_PORT, GPIO_PIN2) == GPIO_INPUT_PIN_LOW)  
        result |= (1 << 1);  // 位2: P4.2
		if (MAP_GPIO_getInputPinValue(BUMP_PORT, GPIO_PIN3) == GPIO_INPUT_PIN_LOW)  
        result |= (1 << 2);  // 位2: P4.3
    if (MAP_GPIO_getInputPinValue(BUMP_PORT, GPIO_PIN5) == GPIO_INPUT_PIN_LOW)
        result |= (1 << 3);  // 位3: P4.5
    if (MAP_GPIO_getInputPinValue(BUMP_PORT, GPIO_PIN6) == GPIO_INPUT_PIN_LOW)
        result |= (1 << 4);  // 位4: P4.6  
    if (MAP_GPIO_getInputPinValue(BUMP_PORT, GPIO_PIN7) == GPIO_INPUT_PIN_LOW)
        result |= (1 << 5);  // 位5: P4.7
    
		BUMP_DATA = result;
    
    return result;
}

void Get_Bump_Update()
{

	float score = 0;
	float valid = 0;
	for(uint8_t i=0; i<6; i++)
	{
		if(BUMP_DATA & (0x01<<i))
		{
			valid += 1;
			score += (i-2.5f);
		}
	}
	if(valid != 0)
	{
		float err = score/valid;
		BUMP_ERR = (err>0)?(5-err):(-5+err);
	}
	BUMP_VALID = valid;
}

