#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "IR_Sensor.h"
#include "PWM.h"  // 
#include "led.h"  // 用于避障指示
#include "sysinit.h"


//全局变量定义
uint16_t resultsBuffer[3];
uint16_t distanceBuffer[3];

// 红外传感器校准系数（示例值，需根据实际校准调整）
#define A   1195172   // 左传感器A系数
#define B   -1058     // 左传感器B系数
#define C   0        // 左传感器C系数（参考点偏移）

uint8_t IR_VALID = 0;
float IR_ERR = 0;

// 红外传感器初始化
void IR_Sensor_Init(void)
{
		/* 启用浮点运算的FPU */
    MAP_FPU_enableModule();
    MAP_FPU_enableLazyStacking();
	
    /* 红外传感器引脚配置:
     * 左红外传感器:P9.1 (ADC输入A16）
     * 右红外传感器:P9.0 (ADC输入 A17)  
     * 前红外传感器:P4.1 (ADC?输入A12)
     */
    
       // 配置左传感器引脚 P9.1 为ADC功能
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P9, 
                                                   GPIO_PIN1, 
                                                   GPIO_TERTIARY_MODULE_FUNCTION);
    
    // 配置右传感器引脚 P9.0 为ADC功能
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P9, 
                                                   GPIO_PIN0, 
                                                   GPIO_TERTIARY_MODULE_FUNCTION);
    
    // 配置前传感器引脚 P4.1 为ADC功能
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, 
                                                   GPIO_PIN1, 
                                                   GPIO_TERTIARY_MODULE_FUNCTION);
   // 初始化ADC14
    MAP_ADC14_enableModule();
    MAP_ADC14_initModule(  ADC_CLOCKSOURCE_MCLK,    // 改用MCLK（48MHz），避免ADCOSC不稳定
        ADC_PREDIVIDER_4,       // 预分频
        ADC_DIVIDER_5,           // 分频
        0);
    
   // 配置ADC内存（3个通道）
	 MAP_ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM2, true);
    // 右传感器 - P9.0 - A17 - ADC_MEM0

    MAP_ADC14_configureConversionMemory(ADC_MEM0, 
                                       ADC_VREFPOS_AVCC_VREFNEG_VSS, 
                                       ADC_INPUT_A17, false);
    
    // 前传感器 - P4.1 - A12 - ADC_MEM1

    MAP_ADC14_configureConversionMemory(ADC_MEM1,
                                       ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                       ADC_INPUT_A12, false);
    
    // 左传感器 - P9.1 - A16 - ADC_MEM2

    MAP_ADC14_configureConversionMemory(ADC_MEM2,
                                       ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                       ADC_INPUT_A16, false);
    
     // 使能ADC中断
    MAP_ADC14_enableInterrupt(ADC_INT2);
    MAP_Interrupt_enableInterrupt(INT_ADC14);
     // 使能ADC
    MAP_ADC14_enableConversion();
		
		 MAP_ADC14_enableSampleTimer(ADC_MANUAL_ITERATION); //手动触发采样
}
void ADC14_IRQHandler(void)
{
    uint_fast64_t status = MAP_ADC14_getEnabledInterruptStatus();
    MAP_ADC14_clearInterruptFlag(status);
	
    if (ADC_INT2 & status)
    {
        MAP_ADC14_getMultiSequenceResult(resultsBuffer);  
    }
}
// 获取红外传感器距离值
void IR_GetDistances(uint16_t distances[3])
{

  
        // 处理右传感器（ADC_MEM0）
        if (resultsBuffer[0] < 50) {
            distances[0] = 800;
        } else {

            // 双曲线模型：distance = A/(ADC_value + B) + C
            distances[0] = (uint16_t)(A / (resultsBuffer[0] + B) + C);
            
            // 限制在有效范围内（70mm~800mm）
            if (distances[0] > 800) distances[0] = 800;
            //if (distances[0] < 70) distances[0] = 70;
        }
        
        // 处理前传感器（ADC_MEM1）
        if (resultsBuffer[1] < 50) {
            distances[1] = 800;
        } else {

            distances[1] = (uint16_t)(A / (resultsBuffer[1] + B) + C);
            
            if (distances[1] > 800) distances[1] = 800;
            //if (distances[1] < 70) distances[1] = 70;
        }
        
        // 处理左传感器（ADC_MEM2）
        if (resultsBuffer[2] < 50) {
            distances[2] = 800;
        } else {

            distances[2] = (uint16_t)(A / (resultsBuffer[2] + B) + C);
            
            if (distances[2] > 800) distances[2] = 800;
            //if (distances[2] < 70) distances[2] = 70;
        }
  
}
void Get_Distance_Update()
{
	float score = 0;
	float valid = 0;
		for(uint8_t i=0; i<3; i++)
	{
		if(distanceBuffer[i] <= 150)
		{
			valid += 1;
			score += (i-1.0f);
		}
	}
	if(valid != 0)
	{
		IR_ERR = score;
		
	}
	IR_VALID = valid;
	
}

