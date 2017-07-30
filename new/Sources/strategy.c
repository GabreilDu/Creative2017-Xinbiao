/*
 * strategy.c
 *
 *  Created on: May 24, 2016
 *      Author: lenovo
 */

#include "includes.h"

unsigned char s_data=3,s_data1=3,s_data2=6,k_data=0,k_data1=0,k_data2=0;
unsigned char S[4]={0,0,0,0},S1[3]={0,0,0},S2[3]={0,0,0},k[4]={0,0,0,0},k1[3]={0,0,0},k2[3]={0,0,0};
unsigned char lamp_turn=10,lamp_turn1=0,lamp_turn2=0;   //�Ƶ����
unsigned char switch_flag=0;  //ת����־λ
//********************2017��������******************************************
extern int velocity;
extern int right_turn;
extern int left_turn;
extern int straight_drive;
extern int car_stop;
extern int car_go_back;//��������
extern int speed_change;
extern int car_turn_around;//��ͷ����
extern int uturn;
extern int target_lost;
extern int target_access;
extern int message_received;
extern int barrier_left_detected;
extern int barrier_right_detected;
extern int stuck1,stuck2;
extern int delay_count;
extern int barrier_offset;
extern int test_pid;
extern int test_helm;
extern double light_offset;
extern int yanshi;
extern int count;
extern long angle;
extern short steer_rate;
extern short speed_number;
extern short angle_rate;
int dasspeed=150;//ʶ��ʱ�ٶ�ԭpwm300�����������
int last1,last2,last3,last4,last5=0;

void Strategy_Switch(void)  //�����л� 
{return;
}
void Strategy_Judge(void)  //�ж�����ת
{return;
}
void Strategy_Input(void)  //�������ֵ
{
	return;
}
void Strategy_Input2(void)
{
	return;
}
void Strategy_Input3(void)
{
	return;
}
void keep_Input(void)  //�������ֵ
{
	return;
}
void keep_Input1(void)
{
	return;
}
void keep_Input2(void)
{
	return;
}
#if 1
void control_car_action(void)
{	
	if(speed_change==1)//�޸��ٶ�
	{
		speed_change=0;
		velocity=speed_number;
		if(last1==1)
		{
		   last1=0;
		   right_turn=1;
		}
		else if(last2==1)
		{
		   last2=0;
		   straight_drive=1;
		}
		else if(last3==1)
		{
		   last3=0;
		   car_stop=1;
		}
		else if(last4==1)
		{
		   last4=0;
		   left_turn=1;
		}
	}
		if(test_helm==1)//ceshi
        {
			test_helm=0;
        	//SET_steer(CENTER-(CENTER-RIGHT)*steer_rate/42+light_offset*75);       	
        	//SET_steer(CENTER-(CENTER-RIGHT)*steer_rate/42.0);
        	SET_steer(steer_rate);
        	SET_motor(500,500);
        	delay_ms(3000);
        	SET_motor(0,0);
//        	set_speed_pwm(dasspeed);
//        	set_speed_target(velocity);
        	
        }
//
////        if(stuck1>100||stuck2>50)//����
////        {
////        	stuck1=0;
////        	stuck2=0;
////        	//last5=1;
////        	SET_steer(CENTER);
////        	set_speed_target(-200);
////        	delay_ms(1500);
////        	set_speed_target(200);
////        }
//        if(car_stop==1)//ͣ��
//        {
//        	car_stop=0;
//        	last3=1;
//        	LCD_Write_Num(105,1,3,2);
//            SET_steer(CENTER);
//            set_speed_target(0);	
//        }
//
//        if(left_turn==1)//��ת �������Ϻ��ҵ�
//        {
//            left_turn=0;
//            last4=1;
//            LCD_Write_Num(105,1,4,2);
//            SET_steer((LEFT-CENTER)*steer_rate/42+CENTER-light_offset*75);
//            //SET_steer((LEFT-CENTER)*steer_rate/42.0+CENTER);
//            if(yanshi==1)
//            {
//                yanshi=0;
//            	delay_ms(1000);
//            }
//            //set_speed_pwm(dasspeed);
//            set_speed_target(velocity);	
//        }
//
	    if(left_turn==1||right_turn==1)
	    {
	    	left_turn=0;
	    	right_turn=0;
	    	targetspeed=80;
	    	SteerControl();	    	
	    	//set_speed_pwm(700);
//	    	set_speed_target(velocity);
	    	if(yanshi==1)
	    	{
	    	   yanshi=0;
	    	   delay_ms(200);
	    	}
	    }
        if(message_received==1)//�յ��ҵ����� ��ʧ����
        {
        	message_received=0;
        	target_lost=0;
        }
        if(target_lost > 35)//��ʧ
        {
        	target_lost=0;
        	targetspeed=0;
//        	SET_steer(RIGHT);
//        	set_speed_target(150);
//        	SET_steer(3050);
//          set_speed_target(-150);
        	SET_steer(CENTER);
//        	set_speed_target(velocity);
        	//set_speed_pwm(0);
            delay_ms(2000);
        	
        }
        if(barrier_left_detected)//������ϰ�
        {
//        	targetspeed=120;
        	barrier_left_detected=0;
        	//SET_steer(CENTER-(CENTER-RIGHT)*angle_rate/60-150);
        	//SET_steer(3600);
        	SET_steer(RIGHT+50);
        	targetspeed=100;
 //       	SteerControl();	       	
        	//set_speed_pwm(350);
 //       	set_speed_target(velocity);
        	//delay_ms(50);
        	delay_ms(70);
        }
        if(barrier_right_detected)//�ұ����ϰ�
        {
//        	targetspeed=120;
        	barrier_right_detected=0;
        	//SET_steer((LEFT-CENTER)*angle_rate/60+CENTER+150);
        	//SET_steer(3000);
        	SET_steer(LEFT-50);
        	targetspeed=100;
//        	SteerControl();	
        	//set_speed_pwm(350);
        	//delay_ms(50);
 //       	set_speed_target(velocity);
            delay_ms(70);
        }
       if(stuck2==1||!collision_switch1)
//        if(stuck2==1)
        {
        	//stuck1=0;
        	stuck2=0;
        	count=0;
        	targetspeed=-130;
        	SET_steer(CENTER );
//        	set_speed_target(-200);
        	//set_speed_pwm(-400);
        	delay_ms(500);
        	targetspeed=0;
//        	set_speed_target(0);
        }
//        if(car_turn_around==1)//��ͷ
//        {
//        	car_turn_around=0;
//        	Car_UTurn();//����������action.c
//        }  
//        if(target_access)
//        {
//        	target_access=0;
//        	targetspeed=130;
//        }        
//        if(straight_drive==1)//ֱ��
//        {
//        	straight_drive=0;
//        	last2=1;
//        	LCD_Write_Num(105,1,2,2);
//        	SET_steer(CENTER);
//        	//set_speed_pwm(dasspeed);	
//        	set_speed_target(velocity);	
//       }
//        if(test_pid==1)
//        {
//        	test_pid=0;
//        	SET_steer(CENTER);
//        	set_speed_target(275);
//        	delay_ms(5000);
//        	//SET_steer(LEFT);
//        	//set_speed_target(100);
//        	//delay_ms(5000);
//        	SET_steer(CENTER);
//        	set_speed_target(-275);
//        	delay_ms(5000);
//        	set_speed_target(0);
//        }
}
#endif
