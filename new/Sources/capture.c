/*
 * capture.c
 *
 *  Created on: Jun 6, 2016
 *      Author: lenovo
 */

#include "includes.h"

int A[128]={0};
int a_start=43,a_end=82;//扫描起始点
int a_scan=4;//两个比较点的差值
int a_value=115;//跳边沿判断阈值
int al_up_flag=0,ar_down_flag=0;//上升沿，下降沿判定
int al_edge=0,ar_edge=0;//左右跳边沿坐标
int a_barrier_T=5;//障碍物宽度判断阈值
int a_barieer_flag=0;//发现障碍物标志
double barrier_centre;
void ImageCapture(int R[]) 
{
	return;
}

void BarrierJudge(void)
{
	return;
}
