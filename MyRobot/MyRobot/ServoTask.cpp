/*
 * ServoTask.cpp
 *
 * Created: 06.04.2017 18:12:29
 *  Author: ANSO
 */ 

#include <Arduino.h>
#include "ServoTask.h"
#include "DebugMessage.h"

// Public
#define SERVO_H_INTERRUPTION_STEP 30
#ifdef SIMULATOR
	#define SERVO_H_POSITIONING_DELAY 100
#else
	#define SERVO_H_POSITIONING_DELAY 200
#endif
uint32_t Servo_Timer;	

Servo	ServoH;
Servo	ServoV;
uint8_t ServoH_position = 255;	// инициализируем вне диапазона, чтобы при инициализации произошел поворот
boolean Servo_isWaiting = false;

// Private
enum phases{
	SERVO_PHASE_ROTATE=0,
	SERVO_PHASE_STOPED,
	SERVO_PHASE_WAITING};
phases Servo_phase = SERVO_PHASE_WAITING;

//==============================================================
void ServoV_init(void){
	
	ServoV.attach(SERVO_V_PIN);
	ServoV.write(60);		// 100 - max?
	delay(200);
	ServoV.detach();
	DebugMessageSERVO(String(millis()) + F(" ServoV positioned"));
}

//==============================================================
void Servo_Task(void){

	if (millis() >= Servo_Timer) {

		switch(Servo_phase){
			case SERVO_PHASE_ROTATE:

				ServoH.attach(SERVO_H_PIN);
				ServoH.write(ServoH_position + SERVO_H_POS_CORR);

				Servo_phase = SERVO_PHASE_STOPED;
				Servo_Timer = millis() + SERVO_H_POSITIONING_DELAY;		// задержка на окончание поворота, так как нет датчика для этого
				DebugMessageSERVO(String(millis()) + F(" Servo phase: ROTATE. Next position = ") + String(ServoH_position));
				break;

			case SERVO_PHASE_STOPED:
				ServoH.detach();

				Servo_phase = SERVO_PHASE_WAITING;
				Servo_Timer = millis() + SERVO_H_INTERRUPTION_STEP;
				DebugMessageSERVO(String(millis()) + F(" Servo phase: STOPED"));
				break;

			case SERVO_PHASE_WAITING:
				Servo_isWaiting = true;
				Servo_Timer = millis() + SERVO_H_INTERRUPTION_STEP;
				//DebugMessageSERVOLn(String(millis()) + F(" Servo phase: "));
				break;

			default:
				Servo_Timer = millis() + SERVO_H_INTERRUPTION_STEP;		// задержка до следующего обращения
				break;
		}
	}
}

//==============================================================
uint8_t Servo_get_position(void){
	return ServoH_position;	
}

//==============================================================
void Servo_goto_position(uint8_t pos)
{
	if(!Servo_isWaiting) return;
	if(pos != ServoH_position){
		ServoH_position = pos;	
		Servo_phase = SERVO_PHASE_ROTATE;
		Servo_isWaiting = false;
	}
}