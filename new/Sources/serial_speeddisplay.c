/*
 * serial_angledisplay.c
 *
 *  Created on: May 13, 2017
 *      Author: Yulic_zhu
 */

#include "includes.h"
#include "cstdio"
#include "cstring"
#include "math.h"
char* str_reverse(char * source){
	int i;
	static char target[7];//length=6
	for(i=0;i<6;i++)target[i]=source[6-i-1];
	target[6]='\0';
	return target;
}
void Serial__write(char *data, unsigned int n){
	for (; n; n--){		
		LINFLEX_0.UARTSR.B.DTF=1; /* w1c TDRE flag : byte is ready to send*/
		LINFLEX_0.BDRL.B.DATA0 = *data++; /* load data byte */
		/* Wait until Transmit Data Register Empty Flag is set, 
		 * which means last transmit is finished. */
		while(!LINFLEX_0.UARTSR.B.DTF){}
	}
}
void Serial_Speed_Dis(float speed_left,float speed_right){
	int i;
	char message[4][7]={"a","\0","b","\0"};
	speed_left=fabs(speed_left);
	speed_right=fabs(speed_right);
	sprintf(message[1],"%03d%03d",(int)speed_left,(int)((speed_left-(int)speed_left)*1000));
	strcpy(message[1],str_reverse(message[1]));
	
	sprintf(message[3],"%03d%03d",(int)speed_right,(int)((speed_right-(int)speed_right)*1000));
	strcpy(message[3],str_reverse(message[3]));
	
	for(i=0;i<4;i++)Serial__write(message[i], strlen(message[i]));
}
