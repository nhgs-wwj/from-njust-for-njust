#include "route.h"
#include "sysinit.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

uint8_t ROUTE_DATA = 0;
uint8_t ROUTE_VALID = 0;

uint16_t base_compare = 600;

float ROUTE_ERR = 0;

// 初始化
void Route_Init(void)
{
    // IR LED 控制脚
    MAP_GPIO_setAsOutputPin(IR_LED_PORT, IR_LED_PIN);
    MAP_GPIO_setOutputHighOnPin(IR_LED_PORT, IR_LED_PIN);

    // P7.0~P7.7 初始为输入
    MAP_GPIO_setAsInputPin(SENSOR_PORT, SENSOR_PINS);
}

// 读取黑白状态 8-bit (1=黑, 0=白)
uint8_t Route_Read(void)
{
    uint8_t data = 0;
    int i;

    // 1 IR LED 打开（确保环境光影响小）
    MAP_GPIO_setOutputHighOnPin(IR_LED_PORT, IR_LED_PIN);

    // 2 充电：设为输出 & 全部置1
    MAP_GPIO_setAsOutputPin(SENSOR_PORT, SENSOR_PINS);
    MAP_GPIO_setOutputHighOnPin(SENSOR_PORT, SENSOR_PINS);

    delay_us(10); // 充电至少10us

    // 3 放电采样：切回输入模式
    MAP_GPIO_setAsInputPin(SENSOR_PORT, SENSOR_PINS);

    delay_us(900); // 放电时间决定灵敏度，300~1000可调整

    // 4 读取传感器电平
    for(i = 0; i < 8; i++)
    {
        if(MAP_GPIO_getInputPinValue(SENSOR_PORT, (1 << i)))
            data |= (1 << i);
    }
		ROUTE_DATA = data;

    return data;
}

void Get_Route_Update()
{
	float score = 0;
	float valid = 0;
	for(uint8_t i=0; i<8; i++)
	{
		if(ROUTE_DATA & (0x01<<i))
		{
			valid += 1;
			score += (i-3.5f);
		}
	}
	if(valid)
	{
		if(valid <= 2)
		{
			ROUTE_ERR = score/valid;
			base_compare = 600;
		}
		else if(valid > 2)
		{
			ROUTE_ERR = score/valid*1.5f;
			base_compare = 450;
		}
	}
	ROUTE_VALID = valid;
}
