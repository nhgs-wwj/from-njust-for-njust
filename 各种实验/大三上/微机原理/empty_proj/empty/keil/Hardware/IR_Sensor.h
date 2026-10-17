#ifndef IR_SENSOR_H
#define IR_SENSOR_H

#include <stdint.h>

// 避障参数定义 - 参考模块14的碰撞检测
#define IR_OBSTACLE_DISTANCE 100  // 100mm避障距离，根据模块15的传感器特性调整
#define IR_AVOIDANCE_TIME 5000     // 避障动作持续时间5000ms
//声明外部变量
extern uint8_t speed_flag;
extern int compare;

// 全局变量声明
extern volatile uint8_t avoidance_flag;
extern volatile uint32_t avoidance_timer;
extern volatile uint16_t ir_left_value;
extern volatile uint16_t ir_right_value; 
extern volatile uint16_t ir_front_value;

extern uint16_t resultsBuffer[3];
extern uint16_t distanceBuffer[3];

extern uint8_t IR_VALID;
extern float IR_ERR;
// 函数声明
void IR_Sensor_Init(void);
void IR_GetDistances(uint16_t distances[3]);
void Get_Distance_Update(void);

#endif
