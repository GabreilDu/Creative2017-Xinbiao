//315
//
#include "includes.h"
int velocity=0;
int i=0;

extern int csl,csr;
extern int tsl_PWM,tsr_PWM;
extern int mode;

void Mode0_Quick(void);
void Mode3_Stable(void);
void Mode4_Suicide(void);

int main (void)
{
	initALL();
	initPIT();
	delay_ms(1000);
	EMIOS_0.CH[6].CCR.B.FEN=1;  //开场中断 
	if(mode==0)		
		Mode0_Quick();//图像显示屏显示，车速20，显示offset RoadType，舵机打角，wifi_car_action不激活
	else if(mode==3)
		Mode3_Stable();
	else if(mode==4)
			Mode4_Suicide();
}
void Mode0_Quick(void)
{
	g_f_enable_speed_control=1;
	for(;;) 	
{
	
	OLED_Fill(0x00);
	
	OLED_SetPointer(1,0);
	OLED_Str("forwardleft1 ");
	OLED_Num(forwardleft);
	
	OLED_SetPointer(2,0);
	OLED_Str("forwardright2 ");
	OLED_Num(forwardright);
	
	OLED_SetPointer(3,0);
	OLED_Str("csl ");
	OLED_Num(csl);
	
	OLED_SetPointer(4,0);
	OLED_Str("csr ");
	OLED_Num(csr);
	
	OLED_SetPointer(5,0);
	OLED_Str("tsl_PWM ");
	OLED_Num(tsl_PWM);
	
	OLED_SetPointer(6,0);
	OLED_Str("tsr_PWM ");
	OLED_Num(tsr_PWM);
	
	OLED_SetPointer(7,0);
	OLED_Str("MODE ");
	OLED_Num(mode);
//		sending_service_package(0x55,0x0066,0x01F4);
//		delay_ms(50);
	if (REMOTE_FRAME_STATE_OK == g_remote_frame_state)
	{
		g_remote_frame_state = REMOTE_FRAME_STATE_NOK;
		Wifi_Ctrl();			
	}
	control_car_action();			// 全场动作控制
}

	
}
void Mode3_Stable(void)
{
	g_f_enable_speed_control=1;
		for(;;) 	
	{
		
		OLED_Fill(0x00);
		
		OLED_SetPointer(1,0);
		OLED_Str("forwardleft1 ");
		OLED_Num(forwardleft);
		
		OLED_SetPointer(2,0);
		OLED_Str("forwardright2 ");
		OLED_Num(forwardright);
		
		OLED_SetPointer(3,0);
		OLED_Str("csl ");
		OLED_Num(csl);
		
		OLED_SetPointer(4,0);
		OLED_Str("csr ");
		OLED_Num(csr);
		
		OLED_SetPointer(5,0);
		OLED_Str("tsl_PWM ");
		OLED_Num(tsl_PWM);
		
		OLED_SetPointer(6,0);
		OLED_Str("tsr_PWM ");
		OLED_Num(tsr_PWM);
		
		OLED_SetPointer(7,0);
		OLED_Str("MODE ");
		OLED_Num(mode);
	//		sending_service_package(0x55,0x0066,0x01F4);
	//		delay_ms(50);
		if (REMOTE_FRAME_STATE_OK == g_remote_frame_state)
		{
			g_remote_frame_state = REMOTE_FRAME_STATE_NOK;
			Wifi_Ctrl();			
		}
		control_car_action_stable();			// 全场动作控制
	}
}
void Mode4_Suicide(void)
{
	g_f_enable_speed_control=1;
		for(;;) 	
	{
		
		OLED_Fill(0x00);
		
		OLED_SetPointer(1,0);
		OLED_Str("forwardleft1 ");
		OLED_Num(forwardleft);
		
		OLED_SetPointer(2,0);
		OLED_Str("forwardright2 ");
		OLED_Num(forwardright);
		
		OLED_SetPointer(3,0);
		OLED_Str("csl ");
		OLED_Num(csl);
		
		OLED_SetPointer(4,0);
		OLED_Str("csr ");
		OLED_Num(csr);
		
		OLED_SetPointer(5,0);
		OLED_Str("tsl_PWM ");
		OLED_Num(tsl_PWM);
		
		OLED_SetPointer(6,0);
		OLED_Str("tsr_PWM ");
		OLED_Num(tsr_PWM);
		
		OLED_SetPointer(7,0);
		OLED_Str("MODE ");
		OLED_Num(mode);
	//		sending_service_package(0x55,0x0066,0x01F4);
	//		delay_ms(50);
		if (REMOTE_FRAME_STATE_OK == g_remote_frame_state)
		{
			g_remote_frame_state = REMOTE_FRAME_STATE_NOK;
			Wifi_Ctrl();			
		}
		control_car_action_stable();			// 全场动作控制
		if(suicide_counter==3)
		{
			targetspeed=0;
			delay_ms(100000000);
		}
	}
}

