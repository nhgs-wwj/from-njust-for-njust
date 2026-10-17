#ifndef	__PID_H__
#define	__PID_H__

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>


typedef	struct{
	float kp,ki,kd;
	float	value;
	float	err,err_last,err_last_last;
	float	err_integral;
	float 	target;
	float	output;
	//积分限幅
	uint8_t int_lmt_en;
	float max_int;
	float min_int;
	//输出限幅
	uint8_t out_lmt_en;
	float max_out;
	float min_out;
}PID_TypeDef;

float pid_realize_in(PID_TypeDef *pid);//增量式
float pid_realize_lo(PID_TypeDef *pid);//位置式

void pid_clear_err(PID_TypeDef *pid);//清除err



#endif
