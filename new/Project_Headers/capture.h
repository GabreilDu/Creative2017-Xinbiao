/*
 * capture.h
 *
 *  Created on: Jun 6, 2016
 *      Author: lenovo
 */

#ifndef CAPTURE_H_
#define CAPTURE_H_

#define CCDR_SI  SIU.GPDO[34].R   //定义右线性传感器的端口 SI  C2
#define CCDR_CLK SIU.GPDO[36].R   //定义右线性传感器的端口 CLK C4

extern int A[128];
extern int a_barieer_flag;
void ImageCapture(int R[]);
extern void BarrierJudge(void);
extern double barrier_centre;
extern int a_start,a_end;
#endif /* CAPTURE_H_ */
