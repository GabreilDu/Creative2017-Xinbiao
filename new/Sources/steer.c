/*
 * steer.c
 *
 *  Created on: Mar 13, 2016
 *      Author: zhongwei
 */

#include "includes.h"
#include "math.h"
//66-64
/*************************舵机参数***************************/
byte wrong_count=0;
byte aim=0,aim2=0;
byte close_supersonic=1,cycle_flag=0,start_flag=1,menu=0;
byte success=0,straight_flag=10;
byte cycle_i=62,cycle_j=80,turnleft=65,edge=61;//turnleft为近处目标方向，不宜轻易改变//52/65
//**********************2017创意组舵机参数**********************************************
double target_offset=0,last_offset=0;	//舵机偏差值记录
double Steer_kp=0,Steer_kd=0;//舵机P、D值
WORD Steer_PWM[4]={0,0,0,0};//舵机输出值记录
WORD steer=0;
extern long angle;
extern long Distanz;
extern int bz;
extern int leftbackturn,rightbackturn;
/*************************舵机接口函数***********************/
void SET_steer(unsigned int steer)
{EMIOS_0.CH[4].CBDR.R = steer;}
/*************************向左转时舵机error参数设置***********************/
void steer_error_left(void)
{
	return;
}
/*************************向右转时舵机error参数设置***********************/
void steer_error_right(void)
{return;}
/*************************舵机PD参数设置***********************/
void Steer_PDSet(void)
{return;}
/*************************舵机控制向左转U型弯，PD***********************/
void SteerControl_left(void)
{return;}
/*************************舵机控制向右转U型弯，PD***********************/
void SteerControl_right(void)
{return;}
/*************************总速度控制***********************/
void SteerControl()
{
	double adj_rate=asin(2.5/Distanz)*256/360;
	target_offset=angle;
	Steer_kd=0.5;
//	if(ABS(target_offset)>40)
//	{
//		target_offset=1.5*target_offset;
//	}
//	else if(ABS(target_offset)>20)
//		{
//			target_offset=1.5*target_offset;
//		}
//	else if(ABS(target_offset)>0)
//	{
//		target_offset=1.5*target_offset;
//	}
//	else if(target_offset>0&&target_offset<=40)
//	{
//		target_offset=target_offset+adj_rate;
//	}
//	else if(target_offset<0&&target_offset>=-40)
//	{
//		target_offset=target_offset-adj_rate;
//	}
//	target_offset=target_offset-8;


                  //1号车PID参数       dby
if(target_offset>-64&&target_offset<=-30&&Distanz<5)
{
//	Steer_kp=6;
//	target_offset=-100;
	//steer=CENTER+Steer_kp*target_offset+Steer_kd*(target_offset-last_offset);//位置式PD
	steer=RIGHT;
	
}
else if(leftbackturn==1)	
{
	leftbackturn=0;
	steer=LEFT;
}
else if(rightbackturn==1)	
{
	rightbackturn=0;
	steer=RIGHT;
}
else 
	{if(Distanz>10&&ABS(target_offset)<=64)
{
//	if(ABS(target_offset)<16)       {Steer_kp=3.5;}
//	else if(ABS(target_offset)<32)  {Steer_kp=(ABS(target_offset)-16)*0.09375+3.5;}
//	else if(ABS(target_offset)<48)  {Steer_kp=(ABS(target_offset)-32)*0.0625+5;}
//	else if(ABS(target_offset)<64)  {Steer_kp=(ABS(target_offset)-48)*0.0625+6;}
//	else                            {Steer_kp=7;}
	target_offset=target_offset+7;
	Steer_kp=6.5;
}
else
{
	target_offset=target_offset+5;//7 9偏差效果还不错
	Steer_kp=8;
}
	steer=CENTER+Steer_kp*target_offset+Steer_kd*(target_offset-last_offset);//位置式PD
	last_offset=target_offset;
}
	
//	Steer_kp=6.5;
//	if(ABS(target_offset)>40)
//	{
//		target_offset=target_offset*1.5;
//		
//	}


	
	if(steer>LEFT)
	{
		steer=LEFT;
	}
	else if(steer<RIGHT)
	{
		steer=RIGHT;
	}
	//舵机限值+舵机输出
	//set_speed_pwm(velocity);
	SET_steer(steer);
	//LCD_Write_Num(105,5,(int)Steer_PWM[3],4);
	//存舵机值和offset值
	//Steer_PWM[0]=Steer_PWM[1];Steer_PWM[1]=Steer_PWM[2];Steer_PWM[2]=Steer_PWM[3];
	
}
