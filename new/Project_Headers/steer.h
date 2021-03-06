/*
 * steer.h
 *
 *  Created on: Mar 13, 2016
 *      Author: zhongwei
 */

#ifndef STEER_H_
#define STEER_H_

#define RIGHT 3260//右极限角度3400
#define LEFT 4000//左极限角度4310

extern byte wrong_count;
extern byte aim,aim2;
extern byte success;
extern byte start_flag;
extern byte close_supersonic;
extern byte menu;
extern byte cycle_i,cycle_j,turnleft;
extern double target_offset,last_offset,Steer_kp;
extern WORD Steer_PWM[4];

void SET_steer(unsigned int steer);
void steer_error_left(void);
void steer_error_right(void);
extern void Steer_PDSet(void);
extern void SteerControl_left(void);
extern void SteerControl_right(void);
//extern void BarrierJudge(void);
void SteerControl(void);


#endif /* STEER_H_ */
