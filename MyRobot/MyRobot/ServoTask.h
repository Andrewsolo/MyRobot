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

extern Servo servo_h;
extern Servo servo_v;

#define SERVO_H_INTERRUPTION_STEP 30
#define SERVO_H_POSITIONING_DELAY 300

//extern uint8_t servo_h_positions[];
extern uint8_t servo_h_distances[];
extern uint8_t servo_h_position;
#define SERVO_H_POS_CORRECTION 10

#define SERVO_PHASE_GETING_ANGLE	0
#define SERVO_PHASE_STOPED			1
#define SERVO_PHASE_WAITING			2


extern boolean servo_doRotate;
extern boolean servo_doWait;
extern boolean servo_isWaiting;

extern unsigned long servo_h_rotationTimer;
void Task_ServoHandler(void);
//uint8_t servo_h_getnextposition(void);
uint8_t servo_positions_cnt(void);


#endif /* SERVOTASK_H_ */