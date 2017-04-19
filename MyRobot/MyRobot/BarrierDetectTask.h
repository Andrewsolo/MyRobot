/*
 * BarrierDetectTask.h
 *
 * Created: 14.04.2017 12:54:41
 *  Author: soan1006
 */ 


#ifndef BARRIERDETECTTASK_H_
#define BARRIERDETECTTASK_H_

#include "Arduino.h"

#define BARRIERDETECT_DELAY 50
#define BARRIERDETECT_SPEED_LOWEST MOTOR_SPEED_MIN
#define BARRIERDETECT_SPEED_LOWER (MOTOR_SPEED_MAX+MOTOR_SPEED_MIN)/2
#define BARRIERDETECT_DISTANCE_MIN	30
#define BARRIERDETECT_DISTANCE_MID	50
#define BARRIERDETECT_DISTANCE_MAX	100


typedef struct strDistanceMeas{uint8_t Position; uint8_t Distance;};
extern strDistanceMeas barrierdetect_points[];// = {90, 0, 135, 0, 90, 0, 45, 0};


extern unsigned long barrierdetect_Timer;
//extern boolean barrierdetect_isEnabled;
extern int barrierdetect_distance;

void barrierdetect_init(void);
void Task_BarrierDetection(void);
uint8_t barrierdetect_get_servo_positions_cnt(void);
void barrierdetect_enable(void);
void barrierdetect_disable(void);
#endif /* BARRIERDETECTTASK_H_ */