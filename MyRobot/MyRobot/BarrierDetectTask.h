/*
 * BarrierDetectTask.h
 *
 * Created: 14.04.2017 12:54:41
 *  Author: soan1006
 */ 


#ifndef BARRIERDETECTTASK_H_
#define BARRIERDETECTTASK_H_

#include "Arduino.h"

#define BARRIERDETECT_SPEED_LOWEST MOTOR_SPEED_MIN
#define BARRIERDETECT_SPEED_LOWER (MOTOR_SPEED_MAX+MOTOR_SPEED_MIN)/2
#define BARRIERDETECT_DISTANCE_MIN	30
#define BARRIERDETECT_DISTANCE_MID	40
#define BARRIERDETECT_DISTANCE_MAX	50


typedef struct strDistanceMeas{uint8_t Position; uint8_t Distance;};
extern strDistanceMeas BD_points[];// = {90, 0, 135, 0, 90, 0, 45, 0};


extern uint32_t BD_Timer;
extern int BD_distance;
extern boolean BD_isDistanceCalculated;

void	BD_init(void);
void	BD_Task(void);
uint8_t BD_get_distance();
void	BD_enable(void);
void	BD_disable(void);
#endif /* BARRIERDETECTTASK_H_ */