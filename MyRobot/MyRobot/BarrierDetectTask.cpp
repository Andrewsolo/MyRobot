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
boolean barrierdetect_isEnabled = false;

strDistanceMeas barrierdetect_points[] = {90, 0, 135, 0, 90, 0, 45, 0};
#define BARRIERDETECT_PARK_POSITION barrierdetect_points[0].Position


int barrierdetect_distance = 0;										// TODO перенести расчет sonar_distance в модуль sonar или servo
uint8_t barrierdetect_pos_num = 0;

#define BARRIERDETECT_PHASE_IDLE				0
#define BARRIERDETECT_PHASE_SERVO_POSITIONING	1
#define BARRIERDETECT_PHASE_SERVO_WAIT_STOP		2
#define BARRIERDETECT_PHASE_SONAR_PING			3
#define BARRIERDETECT_PHASE_SONAR_WAIT_ECHO		4
#define BARRIERDETECT_PHASE_CALCULATION			5
#define BARRIERDETECT_PHASE_REACTION			6
#define BARRIERDETECT_PHASE_SERVO_PARKING		7
#define BARRIERDETECT_PHASE_SERVO_WAIT_PARKED	8
uint8_t barrierdetect_phase = BARRIERDETECT_PHASE_IDLE;

uint8_t	barrierdetect_get_distance(void);
void	barrierdetect_motor_speed_down(void);

//==============================================================
void barrierdetect_init(void){
	
	//servo_go_position(BARRIERDETECT_PARK_POSITION);
	
}

//==============================================================
void Task_BarrierDetection(void){

	if (millis() >= barrierdetect_Timer) {
		barrierdetect_Timer += BARRIERDETECT_DELAY;
		
		switch(barrierdetect_phase){
			case BARRIERDETECT_PHASE_IDLE:
				if(barrierdetect_isEnabled)
					barrierdetect_phase = BARRIERDETECT_PHASE_SERVO_POSITIONING;
				else
					if(servo_get_position()!=BARRIERDETECT_PARK_POSITION)
						barrierdetect_phase = BARRIERDETECT_PHASE_SERVO_PARKING;
				break;
				
			case BARRIERDETECT_PHASE_SERVO_POSITIONING:
				servo_go_position(barrierdetect_points[barrierdetect_pos_num].Position);	
				
				
				barrierdetect_phase = BARRIERDETECT_PHASE_SERVO_WAIT_STOP;
				break;
				
			case BARRIERDETECT_PHASE_SERVO_WAIT_STOP:
				if(servo_isWaiting) barrierdetect_phase = BARRIERDETECT_PHASE_SONAR_PING;
				break;
								
			case BARRIERDETECT_PHASE_SONAR_PING:
				sonar_isPingEnabled = true;			
				barrierdetect_phase = BARRIERDETECT_PHASE_SONAR_WAIT_ECHO;			
				break;
						
			case BARRIERDETECT_PHASE_SONAR_WAIT_ECHO:
				if(sonar_isEchoChecked){
					barrierdetect_points[barrierdetect_pos_num].Distance = sonar_ping_result;					
					if(++barrierdetect_pos_num > barrierdetect_get_servo_positions_cnt()) barrierdetect_pos_num = 0; 
					barrierdetect_phase = BARRIERDETECT_PHASE_CALCULATION;
				}
				break;		
				
			case BARRIERDETECT_PHASE_CALCULATION:
				barrierdetect_distance = barrierdetect_get_distance();
				
				Serial.print(F("Sonar distance: "));
				Serial.println(String(barrierdetect_distance) + F("cm"));
				
				barrierdetect_phase = BARRIERDETECT_PHASE_REACTION;
				break;								

			case BARRIERDETECT_PHASE_REACTION:
				barrierdetect_motor_speed_down();
				
				barrierdetect_phase = BARRIERDETECT_PHASE_IDLE;
				break;

			case BARRIERDETECT_PHASE_SERVO_PARKING:
				servo_go_position(BARRIERDETECT_PARK_POSITION);
						
				barrierdetect_phase = BARRIERDETECT_PHASE_SERVO_WAIT_PARKED;
				break;

			case BARRIERDETECT_PHASE_SERVO_WAIT_PARKED:
				if(servo_isWaiting) barrierdetect_phase = BARRIERDETECT_PHASE_IDLE;
				break;

								
			default:
				break;			
		}


	}
}

//==============================================================
// Нахождение минимального значения дистанции
// TODO перенести расчет sonar_distance в модуль sonar или servo
uint8_t barrierdetect_get_distance(void){
	uint8_t distance = SONAR_MAX_DISTANCE;
		
	for (uint8_t i=0;i<barrierdetect_get_servo_positions_cnt();i++){
		if (barrierdetect_points[i].Distance < distance) distance = barrierdetect_points[i].Distance;
	}
	return distance;
}

//==============================================================
// Замедление в зависимости от расстояния до препятствия
void barrierdetect_motor_speed_down(void){
	
	if (barrierdetect_distance >= BARRIERDETECT_DISTANCE_MAX){
		motors_set_max_speed(get_motors_max_speed(), false);
	}
	else if (barrierdetect_distance < BARRIERDETECT_DISTANCE_MIN && get_motors_max_speed() > BARRIERDETECT_SPEED_MIN) {
		motors_set_max_speed(BARRIERDETECT_SPEED_MIN, true);
	}
	else if (barrierdetect_distance < BARRIERDETECT_DISTANCE_MAX && get_motors_max_speed() > BARRIERDETECT_SPEED_MID) {
		motors_set_max_speed(BARRIERDETECT_SPEED_MID, true);
	}
}

//==============================================================
uint8_t barrierdetect_get_servo_positions_cnt(void)
{
	return sizeof(barrierdetect_points)/sizeof(strDistanceMeas);

}