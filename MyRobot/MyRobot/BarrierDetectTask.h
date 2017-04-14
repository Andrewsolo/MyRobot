/*
 * BarrierDetectTask.h
 *
 * Created: 14.04.2017 12:54:41
 *  Author: soan1006
 */ 


#ifndef BARRIERDETECTTASK_H_
#define BARRIERDETECTTASK_H_

#include "Arduino.h"
#include "Motors.h"


#define BARRIERDETECT_DELAY 50
#define BARRIERDETECT_SPEED_MIN MOTOR_SPEED_MIN
#define BARRIERDETECT_SPEED_MID (MOTOR_SPEED_MAX+MOTOR_SPEED_MIN)/2
#define BARRIERDETECT_DISTANCE_MIN	50
#define BARRIERDETECT_DISTANCE_MAX	100

extern unsigned long barrierdetect_Timer;

void Task_BarrierDetection(void);

#endif /* BARRIERDETECTTASK_H_ */