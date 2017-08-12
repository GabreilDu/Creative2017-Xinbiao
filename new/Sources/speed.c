/*
 * speed.c
 *
 *  Created on: Mar 13, 2016
 *      Author: zhongwei
 */

#include "includes.h"
#include "serial_speeddisplay.h"
/*-----------------------------------------------------------------------*/
/* 2017����˫������� 	                                                                      */
/*-----------------------------------------------------------------------*/
int csl=0,csr=0;//currentspeedleft=0,currentspeedright=0;
int tsl=0,tsr=0;//targetspeedleft=0,targetspeedright=0;
int targetspeed=0; 
int	Motor_PWM_MAX=1000; 
int	Motor_PWM_MIN=-1000;
int cyclespeedleft1=80,cyclespeedright1=60,cyclespeedleft2=80,cyclespeedright2=50;//1��ת115 95 2��ת115 75
unsigned int speedcounter1=0,speedcounter2=0,speedcounter3=0,speedcounter4=0;
//**********************���ٲ���***************************/
signed int Speed_kc=15000;
signed int wheel_distance=9;//�복��8
signed int RPID=0;
double r=0;
//**********************���PID����**********************************************;	
double Speed_kp_Left=7,Speed_ki_Left=0.15,Speed_kd_Left=0;//12,0.6
double Speed_kp_Right=7,Speed_ki_Right=0.15,Speed_kd_Right=0;	//12,0.85

int ErrorLeft=0,PreErrorLeft=0,Pre2ErrorLeft=0,SumErrorLeft=0,ErrorRight=0,PreErrorRight=0,Pre2ErrorRight=0,SumErrorRight=0;
int intErrorLeft=0,intErrorRight=0;
float Pwm_Delta_Left=0,Pwm_Delta_Right=0; 
int tsl_PWM=0,tsr_PWM=0,tsr_Delta=0,error_Delta=0;
int tsl_old=0,tsr_old=0;
extern long angle;
extern long Distanz;
/*************************����ӿں���*********************/
void SET_motor(int leftSpeed,int rightSpeed)
{
	if(leftSpeed<=0) {EMIOS_0.CH[20].CBDR.R = 0;EMIOS_0.CH[19].CBDR.R =-leftSpeed;}
	else {EMIOS_0.CH[20].CBDR.R = leftSpeed;EMIOS_0.CH[19].CBDR.R =-0;}//����  E5����   E6���
	if(rightSpeed<=0) {EMIOS_0.CH[22].CBDR.R = -rightSpeed;EMIOS_0.CH[21].CBDR.R = 0;}
	else {EMIOS_0.CH[22].CBDR.R = 0;EMIOS_0.CH[21].CBDR.R = rightSpeed;}//����  E3�ҽ�   E4����
}
/*************************�ٶȿ��ƺ���*********************/
//void Speed_control(void)
//{
//	RPID=CENTER-Steer_PWM[3];
//	r=Speed_kc/RPID;
//	tsr=((r-wheel_distance)/r)*targetspeed;//���ּ���
//	tsl=((r+wheel_distance+2)/r)*targetspeed;//���ּ���
//	SET_motor(tsl,tsr);
//}

/*************************����������***********************/
void SpeedCount(void)
{
	speedcounter1=EMIOS_0.CH[24].CCNTR.R;              //��D12
	if(speedcounter1<speedcounter2)
		csl=speedcounter1+65535-speedcounter2;         //current speed left
	else 
		csl=speedcounter1-speedcounter2;
	if(forwardleft)
		csl=csl;
	else 
		csl=-csl;
	speedcounter2=speedcounter1;
	speedcounter3=EMIOS_0.CH[8].CCNTR.R;               //��A8
	if(speedcounter3<speedcounter4)
	{
		csr=speedcounter3+65535-speedcounter4;         //current speed right
	}
	else 
		csr=speedcounter3-speedcounter4;	
	if(forwardright) 
		csr=csr;
	else 
		csr=-csr;
	speedcounter4=speedcounter3;
}
//*****************************************************************************************************************
//************************************************���ֲ���PID�ٶȿ���************************************************    	  *
//*****************************************************************************************************************
void SpeedControl(void)//�ٶȿ�������ʽ
{
//	static tsl_old,tsr_old;
	if(TargetSteer==RIGHT)
	{
		tsl=cyclespeedleft1;
		tsr=cyclespeedright1;
	}
	else if(TargetSteer==LEFT)
	{
		tsr=cyclespeedleft2;
		tsl=cyclespeedright2;
	}
	else
	{
		tsl=targetspeed;
		tsr=targetspeed;
	}
	
//	if(Distanz>10&&angle<25&&angle>-25)
//	{
//		Speed_kp_Left=10;Speed_ki_Left=0.3;Speed_kd_Left=0;//����120PID8,0.1��0��
//		Speed_kp_Right=10;Speed_ki_Right=0.3;Speed_kd_Right=0;//����120PID8,0.1��0��
//	}
	
	ErrorLeft=tsl-csl;
	ErrorRight=tsr-csr;
    tsl_PWM+=(int)(Speed_kp_Left*(ErrorLeft-PreErrorLeft)+Speed_ki_Left*ErrorLeft+Speed_kd_Left*(ErrorLeft+Pre2ErrorLeft-2*PreErrorLeft));
	
	tsr_Delta=(int)(Speed_kp_Right*(ErrorRight-PreErrorRight)+Speed_ki_Right*ErrorRight+Speed_kd_Right*(ErrorRight+Pre2ErrorRight-2*PreErrorRight));
	tsr_PWM+=tsr_Delta;
//if(TargetSteer<CENTER+50&&TargetSteer>CENTER-50)	
//{
//	if(tsl_old==0)tsl_old=tsl_PWM;				//��ͨ���� 0.5old0.5new�����ٶȿ������ȶ�
//	tsl_PWM = 0.3*tsl_PWM + 0.7*tsl_old;			
//	tsl_old = tsl_PWM;
//	if(tsr_old==0)tsr_old=tsr_PWM;				//��ͨ�ҵ��
//	tsr_PWM = 0.3*tsr_PWM + 0.7*tsr_old;			
//	tsr_old = tsr_PWM;
//}
//else
//{
//	if(tsl_old==0)tsl_old=tsl_PWM;				//��ͨ���� 0.5old0.5new�����ٶȿ������ȶ�
//	tsl_PWM = 0.5*tsl_PWM + 0.5*tsl_old;			
//	tsl_old = tsl_PWM;
//	if(tsr_old==0)tsr_old=tsr_PWM;				//��ͨ�ҵ��
//	tsr_PWM = 0.50*tsr_PWM + 0.5*tsr_old;			
//	tsr_old = tsr_PWM;
//}
	if(tsl_PWM>Motor_PWM_MAX)  tsl_PWM=Motor_PWM_MAX;
	else if(tsl_PWM<Motor_PWM_MIN)  tsl_PWM=Motor_PWM_MIN;	    
	if(tsr_PWM>Motor_PWM_MAX)  tsr_PWM=Motor_PWM_MAX;
	else if(tsr_PWM<Motor_PWM_MIN)  tsr_PWM=Motor_PWM_MIN;
	
	SET_motor(tsl_PWM,tsr_PWM);
	Pre2ErrorLeft=PreErrorLeft;
	Pre2ErrorRight=PreErrorRight;
	PreErrorLeft=ErrorLeft;
	PreErrorRight=ErrorRight;
}

