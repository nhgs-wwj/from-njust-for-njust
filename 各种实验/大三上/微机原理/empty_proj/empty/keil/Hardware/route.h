#ifndef _ROUTE_H_
#define _ROUTE_H_

#include <stdint.h>

#define SENSOR_PORT GPIO_PORT_P7
#define SENSOR_PINS 0xFF   // P7.0~P7.7

#define IR_LED_PORT GPIO_PORT_P5
#define IR_LED_PIN  GPIO_PIN3

extern uint8_t ROUTE_DATA;
extern uint8_t ROUTE_VALID;
extern uint16_t base_compare;
extern float ROUTE_ERR;

typedef enum
{
    ROUTE_CENTER = 0,
    ROUTE_LEFT,
    ROUTE_RIGHT,
    ROUTE_LOST
} RouteStatus;

void Route_Init(void);
uint8_t Route_Read(void);   // 读取8位黑白数据

void Get_Route_Update(void);
#endif
