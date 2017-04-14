/*
 * ServoTask.cpp
 *
 * Created: 06.04.2017 18:12:29
 *  Author: ANSO
 */ 

#include <Arduino.h>
#include "ServoTask.h"
//#include "Servo.h"

// Public
Servo servo_h;
Servo servo_v;
uint8_t servo_h_positions[] = {90, 135, 90, 45};
uint8_t servo_h_distances[sizeof(servo_h_positions)];
uint8_t servo_h_position = 0;
unsigned long servo_h_rotationTimer;	
	
boolean servo_doRotate = false;
boolean servo_doWait = false;
boolean servo_isWaiting = false;
//boolean isServoPositioned = false;

// Private
uint8_t ServoPhase = SERVO_PHASE_WAITING;
uint8_t servo_h_getnextposition(void);


void Task_ServoHandler(void){
	if (millis() >= servo_h_rotationTimer) {


		switch(ServoPhase){
			case SERVO_PHASE_GETING_ANGLE:
				if (servo_doRotate){
					servo_doRotate = false;
					//isServoPositioned = false;

					servo_h.attach(SERVO_H_PIN);
					uint8_t nextpos = servo_h_getnextposition();
					servo_h.write(nextpos);

					ServoPhase = SERVO_PHASE_STOPED;
					servo_h_rotationTimer = millis() + SERVO_H_POSITIONING_DELAY;		// задержка на окончание поворота, так как нет датчика для этого

					Serial.println(String(millis()) + F("SERVO_PHASE_GETING_ANGLE. Next position = ") + String(nextpos));
				}
				break;

			case SERVO_PHASE_STOPED:
				servo_h.detach();
				//isServoPositioned = true;
				//isSonarStartMeasure = true;
				//isSonarEchoChecked = false;

				ServoPhase = SERVO_PHASE_WAITING;
				servo_h_rotationTimer = millis() + SERVO_H_INTERRUPTION_STEP;

				Serial.println(String(millis()) + F("SERVO_PHASE_STOPED"));
				break;

			case SERVO_PHASE_WAITING:
				if (servo_doRotate){
					servo_isWaiting = false;
					ServoPhase = SERVO_PHASE_GETING_ANGLE;
					servo_h_rotationTimer = millis() + SERVO_H_INTERRUPTION_STEP;

					Serial.println(String(millis()) + F("SERVO_PHASE_WAITING"));
				}
				else
					servo_isWaiting = true;
				break;

			default:
				servo_h_rotationTimer = millis() + SERVO_H_INTERRUPTION_STEP;		// задержка до следующего обращения
				break;
		}
	}
}

uint8_t servo_positions_cnt(void)
{
	return sizeof(servo_h_positions);
}

uint8_t servo_h_getnextposition(void){

	if (++servo_h_position >= sizeof(servo_h_positions))
	servo_h_position = 0;

	return (servo_h_positions[servo_h_position] + SERVO_H_POS_CORRECTION);
}