/*
 * ServoTask.cpp
 *
 * Created: 06.04.2017 18:12:29
 *  Author: ANSO
 */ 

#include <Arduino.h>
#include "ServoTask.h"

// Public
Servo servo_h;
Servo servo_v;

uint8_t servo_h_position = 0;
unsigned long servo_h_rotationTimer;	

boolean servo_isWaiting = false;

// Private
uint8_t ServoPhase = SERVO_PHASE_WAITING;
uint8_t servo_h_getnextposition(void);


void Task_ServoHandler(void){
	uint8_t nextpos;

	if (millis() >= servo_h_rotationTimer) {

		switch(ServoPhase){
			case SERVO_PHASE_PARK:
				//servo_doPark = false;

				servo_h.attach(SERVO_H_PIN);
				nextpos = barrierdetect_points[0].Position + SERVO_H_POS_CORR;
				servo_h.write(nextpos);

				ServoPhase = SERVO_PHASE_STOPED;
				servo_h_rotationTimer = millis() + SERVO_H_POSITIONING_DELAY;		// задержка на окончание поворота, так как нет датчика для этого

				Serial.println(String(millis()) + F("SERVO_PHASE_PARK. Position = ") + String(nextpos));
				break;			
			
			case SERVO_PHASE_GETING_ANGLE:
				//servo_doRotate = false;

				servo_h.attach(SERVO_H_PIN);
				nextpos = servo_h_getnextposition();
				servo_h.write(nextpos);

				ServoPhase = SERVO_PHASE_STOPED;
				servo_h_rotationTimer = millis() + SERVO_H_POSITIONING_DELAY;		// задержка на окончание поворота, так как нет датчика для этого

				Serial.println(String(millis()) + F("SERVO_PHASE_GETING_ANGLE. Next position = ") + String(nextpos));
				break;

			case SERVO_PHASE_STOPED:
				servo_h.detach();

				ServoPhase = SERVO_PHASE_WAITING;
				servo_h_rotationTimer = millis() + SERVO_H_INTERRUPTION_STEP;

				Serial.println(String(millis()) + F("SERVO_PHASE_STOPED"));
				break;

			case SERVO_PHASE_WAITING:
				servo_isWaiting = true;
				servo_h_rotationTimer = millis() + SERVO_H_INTERRUPTION_STEP;
				
				//Serial.println(String(millis()) + F("SERVO_PHASE_WAITING"));
				break;

			default:
				servo_h_rotationTimer = millis() + SERVO_H_INTERRUPTION_STEP;		// задержка до следующего обращения
				break;
		}
	}
}

void servo_do_rotate(void){
	ServoPhase = SERVO_PHASE_GETING_ANGLE;
	servo_isWaiting = false;
}

void servo_do_park(void){
	ServoPhase = SERVO_PHASE_PARK;
	servo_isWaiting = false;
}


uint8_t servo_h_getnextposition(void){

	if (++servo_h_position >= barrierdetect_get_servo_positions_cnt())
	servo_h_position = 0;

	return (barrierdetect_points[servo_h_position].Position + SERVO_H_POS_CORR);
}

