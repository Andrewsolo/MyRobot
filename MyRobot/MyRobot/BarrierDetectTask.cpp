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

//==============================================================
void Task_BarrierDetection(void){

	if (millis() >= barrierdetect_Timer) {
		barrierdetect_Timer += BARRIERDETECT_DELAY;

		int sonar_distance_aver;
		sonar_distance = SONAR_MAX_DISTANCE;				// TODO перенести расчет sonar_distance в модуль sonar или servo
		
		// Найдем минимальное значение
		for (uint8_t i=0;i<servo_positions_cnt();i++){
			if (servo_h_distances[i] < sonar_distance) sonar_distance = servo_h_distances[i];
		}
		Serial.print(F("Sonar distance: "));
		Serial.println(String(sonar_distance) + F("cm"));

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
}