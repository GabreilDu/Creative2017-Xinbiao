/*
 * sensor.h
 *
 *  Created on: May 5, 2017
 *      Author: lenovo
 */

#ifndef SENSOR_H_
#define SENSOR_H_

extern int smog1_flag,smog2_flag;
extern int person_flag;

void Smog_check(void);
void Person_check(void);
void Bee_respond(void);
void Crash_ornot();

#endif /* SENSOR_H_ */
