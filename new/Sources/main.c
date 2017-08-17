//315
//
#include "includes.h"
int velocity=0;

extern int csl,csr;
extern int tsl_PWM,tsr_PWM;
int main (void)
{
	initALL();
	initPIT();
	delay_ms(1000);
	EMIOS_0.CH[6].CCR.B.FEN=1;  //开场中断 
	for(;;) 	
	{
		g_f_enable_speed_control=1;
		
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
