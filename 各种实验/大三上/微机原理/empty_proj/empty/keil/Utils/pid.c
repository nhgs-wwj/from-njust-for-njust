/**
  ******************************************************************************
  * @file    pid.c
  * @author  fytoyu
  * @brief   pid算法
  * @date		 2025.1.18
  ******************************************************************************
  */
	
#include "pid.h"

//pid函数

//输出限幅
void output_limit(PID_TypeDef *pid)
{
	if(pid->out_lmt_en)
	{
		if(pid->output > pid->max_out)
			pid->output = pid->max_out;
		else if(pid->output < pid->min_out)
			pid->output = pid->min_out;
	}
}
//积分限幅
void integral_limit(PID_TypeDef *pid)
{
	if(pid->int_lmt_en)
	{
		if(pid->err_integral > pid->max_int)
			pid->err_integral = pid->max_int;
		else if(pid->err_integral < pid->min_int)
			pid->err_integral = pid->min_int;
	}
}

//位置式
float pid_realize_lo(PID_TypeDef *pid)
{
	//计算误差
	pid->err=pid->target-pid->value;
	//误差积分
	pid->err_integral+=pid->err;
	//积分限幅
	integral_limit(pid);
	//计算输出
	pid->output = pid->kp * pid->err + pid->kd * (pid->err - pid->err_last) + pid->ki * pid->err_integral;
	//输出限幅
	output_limit(pid);
	//记录误差
	pid->err_last = pid->err;
	//返回输出
	return pid->output;
}

//增量式
float	pid_realize_in(PID_TypeDef *pid)
{
	//计算误差
	pid->err=pid->target-pid->value;
	//误差积分
	pid->err_integral+=pid->err;
	//积分限幅
	integral_limit(pid);
	//计算输出
	pid->output += pid->kp * (pid->err - pid->err_last) + pid->kd * (pid->err + pid->err_last_last - 2.0f*pid->err_last) + pid->ki * pid->err;
	//输出限幅
	output_limit(pid);
	//记录误差
	pid->err_last_last=pid->err_last;
	pid->err_last=pid->err;
	//返回输出
	return pid->output;
}

void pid_clear_err(PID_TypeDef *pid)
{
	pid->err = 0;
	pid->err_last = 0;
	pid->err_last_last = 0;
	pid->err_integral = 0;
}
