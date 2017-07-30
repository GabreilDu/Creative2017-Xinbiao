/*
 * pit.c
 *
 *  Created on: Apr 1, 2016
 *      Author: lenovo
 */

#include "includes.h"

unsigned int pitcount0=0,pitcount1=0,pitcount2=0,pitcount3=0,pitcount4=0,pitcount5=0,pitcount6=0;
unsigned int barrier_delay=0;
unsigned int supson_delay1=0,supson_delay2=0;
int g_f_enable_speed_control = 0;	/* 启用速度控制标志位 */
int g_f_enable_supersonic=0;	/* 启用超声探测标志位 */
byte CCD_flag=0;
byte OLEDdisplay_flag=0;



void initPIT(void) 
{                                   //1ms一个控制周期// NOTE:  DIVIDER FROM SYSCLK TO PIT ASSUMES DEFAULT DIVIDE BY 1 
	PIT.PITMCR.R = 0x00000001;       // Enable PIT and configure timers to stop in debug mode 
	PIT.CH[1].LDVAL.R = 800000;      // PIT2 timeout = 800000 sysclks x 1sec/80M sysclks = 10 msec 
	PIT.CH[1].TCTRL.R = 0x000000003; // Enable PIT2 interrupt and make PIT active to count 
	INTC_InstallINTCInterruptHandler(PitISR,60,11); 
//	udelay(1);
//	PIT.CH[2].LDVAL.R = 1000000;      // PIT1 timeout = 6000000 sysclks x 1sec/80M sysclks = 75 msec 
//	PIT.CH[2].TCTRL.R = 0x000000003; // Enable PIT1 interrupt and make PIT active to count 
//	INTC_InstallINTCInterruptHandler(PitISR2,61,11); 
}

void PitISR(void)//10ms一个控制周期
{
	SpeedCount();
	if (g_f_enable_speed_control)
	{
		SpeedControl();
	}
	PIT.CH[1].TFLG.B.TIF = 1;//write 1 to clear PIT1 清除标志位
}

void PitISR2(void)
{
	PIT.CH[2].TFLG.B.TIF = 1;
}
