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
unsigned long servo_h_rotationTimer;	
uint8_t servo_h_position = 255;	// инициализируем вне диапазона, чтобы при инициализации произошел поворот
boolean servo_isWaiting = false;

// Private
enum phases{
	SERVO_PHASE_ROTATE=0,
	SERVO_PHASE_STOPED,
	SERVO_PHASE_WAITING};
phases servo_phase = SERVO_PHASE_WAITING;

uint8_t servo_h_getnextposition(void);

//==============================================================
void Task_ServoHandler(void){

	if (millis() >= servo_h_rotationTimer) {

		switch(servo_phase){
			case SERVO_PHASE_ROTATE:

				servo_h.attach(SERVO_H_PIN);
				servo_h.write(servo_h_position + SERVO_H_POS_CORR);

				servo_phase = SERVO_PHASE_STOPED;
				servo_h_rotationTimer = millis() + SERVO_H_POSITIONING_DELAY;		// задержка на окончание поворота, так как нет датчика для этого
				Serial.println(String(millis()) + F(" SERVO_PHASE_GETING_ANGLE. Next position = ") + String(servo_h_position));
				break;

			case SERVO_PHASE_STOPED:
				servo_h.detach();

				servo_phase = SERVO_PHASE_WAITING;
				servo_h_rotationTimer = millis() + SERVO_H_INTERRUPTION_STEP;
				Serial.println(String(millis()) + F(" SERVO_PHASE_STOPED"));
				break;

			case SERVO_PHASE_WAITING:
				servo_isWaiting = true;
				servo_h_rotationTimer = millis() + SERVO_H_INTERRUPTION_STEP;
				
				//Serial.println(String(millis()) + F(" SERVO_PHASE_WAITING"));
				break;

			default:
				servo_h_rotationTimer = millis() + SERVO_H_INTERRUPTION_STEP;		// задержка до следующего обращения
				break;
		}
	}
}

//==============================================================
uint8_t servo_get_position(void){
	return servo_h_position;	
}

//==============================================================
void servo_goto_position(uint8_t pos)
{
	if(!servo_isWaiting) return;
	if(pos != servo_h_position){
		servo_h_position = pos;	
		servo_phase = SERVO_PHASE_ROTATE;
		servo_isWaiting = false;
	}
}