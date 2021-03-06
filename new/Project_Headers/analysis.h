/*
 * analysis.h
 *
 *  Created on: Mar 15, 2016
 *      Author: zhongwei
 */

#ifndef ANALYSIS_H_
#define ANALYSIS_H_

#define video_t 220
#define video_t2 220

extern byte pix_x,pix_y;
extern byte pix_x2,pix_y2;
extern byte pix_i,pix_j;
extern byte wrong_flag;

int lamp_judge(byte pix_i,byte pix_j);
void central_search_2(byte *pix_i,byte *pix_j,byte x,byte y);

byte point_search(byte *pix_i,byte *pix_j,byte i,byte j,byte line);
byte point_search_2(byte *pix_i,byte *pix_j);
byte point_search_3(byte *pix_i,byte *pix_j,byte m,byte n);
extern void lamp_search(byte *pix_i,byte *pix_x2,byte *pix_y2);
void lamp_search_1(byte *pix_i,byte *pix_j);
void lamp_search_2(byte *pix_i,byte *pix_j);
//extern void lamp_search_far_near(byte *pix_i,byte *pix_j);


#endif /* ANALYSIS_H_ */
