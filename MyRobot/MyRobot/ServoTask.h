/*
 * ServoTask.h
 *
 * Created: 06.04.2017 18:13:22
 *  Author: ANSO
 */


#ifndef SERVOTASK_H_
#define SERVOTASK_H_

#include "Servo.h"
#include "HAL.h"

#include "BarrierDetectTask.h"

//====== SERVO ================================================

#if (!defined (SERVO_H_PIN)) || (!defined SERVO_V_PIN)
#error "SERVO Pins not defined!"
#endif

extern Servo servo_h;
extern Servo servo_v;

#define SERVO_H_INTERRUPTION_STEP 30
#define SERVO_H_POSITIONING_DELAY 300

#define SERVO_H_POS_CORR 10		// Корректировка на погрешность установки серводвигателя
#define SERVO_H_POS_MIN (10 + SERVO_H_POS_CORRECTION)
#define SERVO_H_POS_MAX (170 + SERVO_H_POS_CORRECTION)

#define SERVO_PHASE_PARK			0
#define SERVO_PHASE_GETING_ANGLE	1
#define SERVO_PHASE_STOPED			2
#define SERVO_PHASE_WAITING			3

extern boolean servo_isWaiting;

extern unsigned long servo_h_rotationTimer;

void servo_do_rotate(void);
void servo_do_park(void);
void Task_ServoHandler(void);
uint8_t servo_positions_cnt(void);

#endif /* SERVOTASK_H_ */