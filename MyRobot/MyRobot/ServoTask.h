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

//====== SERVO ================================================

#if (!defined (SERVO_H_PIN)) || (!defined SERVO_V_PIN)
#error "SERVO Pins not defined!"
#endif

#define SERVO_H_POS_CORR 10		// Корректировка на погрешность установки серводвигателя
#define SERVO_H_POS_MIN (10 + SERVO_H_POS_CORRECTION)
#define SERVO_H_POS_MAX (170 + SERVO_H_POS_CORRECTION)

extern boolean servo_isWaiting;
extern unsigned long servo_h_rotationTimer;

void servo_v_init(void);
void Task_ServoHandler(void);
uint8_t servo_get_position(void);
void servo_goto_position(uint8_t);
uint8_t servo_positions_cnt(void);

#endif /* SERVOTASK_H_ */