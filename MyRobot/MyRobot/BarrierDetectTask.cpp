/*
 * BarrierDetectTask.cpp
 *
 * Created: 14.04.2017 12:54:55
 *  Author: soan1006
 */ 

#include "BarrierDetectTask.h"
#include "ServoTask.h"
#include "SonarTask.h"

unsigned long barrierdetect_Timer;
int sonar_distance = 0;										// TODO перенести расчет sonar_distance в модуль sonar или servo

uint8_t barrierdetect_get_distance(void);
void barrierdetect_MotorSpeedDown(void);

//==============================================================
void Task_BarrierDetection(void){

	if (millis() >= barrierdetect_Timer) {
		barrierdetect_Timer += BARRIERDETECT_DELAY;

			 
			 
		sonar_distance = barrierdetect_get_distance();
		Serial.print(F("Sonar distance: "));
		Serial.println(String(sonar_distance) + F("cm"));

		barrierdetect_MotorSpeedDown();

	}
}

//==============================================================
// Нахождение минимального значения дистанции
// TODO перенести расчет sonar_distance в модуль sonar или servo
uint8_t barrierdetect_get_distance(void){
	uint8_t distance = SONAR_MAX_DISTANCE;
	

	for (uint8_t i=0;i<servo_positions_cnt();i++){
		if (servo_h_distances[i] < distance) distance = servo_h_distances[i];
	}
	return distance;
	
}

//==============================================================
// Замедление в зависимости от расстояния до препятствия
void barrierdetect_MotorSpeedDown(void){
	

	if (sonar_distance >= BARRIERDETECT_DISTANCE_MAX){
		motors_set_max_speed(get_motors_max_speed(), false);
	}
	else if (sonar_distance < BARRIERDETECT_DISTANCE_MIN && get_motors_max_speed() > BARRIERDETECT_SPEED_MIN) {
		motors_set_max_speed(BARRIERDETECT_SPEED_MIN, true);
	}
	else if (sonar_distance < BARRIERDETECT_DISTANCE_MAX && get_motors_max_speed() > BARRIERDETECT_SPEED_MID) {
		motors_set_max_speed(BARRIERDETECT_SPEED_MID, true);
	}
	
}