/*
 * strategy.h
 *
 *  Created on: May 24, 2016
 *      Author: lenovo
 */

#ifndef STRATEGY_H_
#define STRATEGY_H_

extern unsigned char s_data,lamp_turn,s_data1,s_data2,k_data,k_data1,k_data2;
extern unsigned char switch_flag;  //ת����־λ
extern int g_f_enable_speed_control;

void Strategy_Switch();
void Strategy_Judge();
void Strategy_Input();
void Strategy_Input2();
void Strategy_Input3();
void keep_Input(void);
void keep_Input1(void);
void keep_Input2(void);
extern void control_car_action(void);
#endif /* STRATEGY_H_ */
