/*
 * debug.h
 *
 *  Created on: Mar 13, 2016
 *      Author: zhongwei
 */

#ifndef DEBUG_H_
#define DEBUG_H_

extern unsigned char *send;
extern unsigned char putstring[];
extern unsigned int Ts;
extern unsigned int Ti;
extern unsigned int Tj;
extern int CurrentSteer;
extern unsigned long time1;
extern unsigned long time2;
extern unsigned long time3;
extern unsigned long time3_2;

extern int X[16];
extern unsigned char Y[25],Z[11];
extern unsigned char RX_data;

extern unsigned char S3_last;
extern unsigned char S4_last;
extern unsigned char S5_last;
extern unsigned char S6_last;
extern unsigned char keymode;
extern unsigned long TimeMesure(void);
void BlueTx(void); 
void BlueTx_CCD(void);
void LINFlex_TX(unsigned char data);
void LINFlex_TX_Interrupt(void);
void LINFlex_RX_Interrupt(void);
void KeyJudge(void);
void KeyDebug_Display(void);





#endif /* DEBUG_H_ */


