/*
 * BarrierDetectTask.cpp
 *
 * Created: 14.04.2017 12:54:55
 *  Author: soan1006
 */ 

#include "BarrierDetectTask.h"
#include "ServoTask.h"
#include "SonarTask.h"
#include "Motors.h"
#include "DebugMessage.h"

#define BARRIERDETECT_DELAY 50
unsigned long barrierdetect_Timer;

strDistanceMeas barrierdetect_points[] = {90, 0, 135, 0, 90, 0, 45, 0};
#define BARRIERDETECT_PARK_POSITION barrierdetect_points[0].Position

int		barrierdetect_distance = 0;										// TODO перенести расчет sonar_distance в модуль sonar или servo
uint8_t barrierdetect_pos_num = 0;
boolean barrierdetect_isEnabled = false;

enum phases{
	BARRIERDETECT_PHASE_IDLE=0,
	BARRIERDETECT_PHASE_SERVO_POSITIONING,
	BARRIERDETECT_PHASE_SERVO_WAIT_STOP,
	BARRIERDETECT_PHASE_SONAR_PING,
	BARRIERDETECT_PHASE_SONAR_WAIT_ECHO,
	BARRIERDETECT_PHASE_CALCULATION,
	BARRIERDETECT_PHASE_REACTION,
	BARRIERDETECT_PHASE_SERVO_PARKING,
	BARRIERDETECT_PHASE_SERVO_WAIT_PARKED};
phases barrierdetect_phase = BARRIERDETECT_PHASE_IDLE;

uint8_t	barrierdetect_calculate_distance(void);
void	barrierdetect_motor_speed_limitation(void);
uint8_t barrierdetect_get_servo_positions_cnt(void);

//==============================================================
void barrierdetect_init(void){
	
#ifndef SIMULATOR	
	servo_v_init();
#endif	
	
	//servo_go_position(BARRIERDETECT_PARK_POSITION);
	//DebugMessageLn(String(millis()) + F(" BD started"));
	
}

//==============================================================
void Task_BarrierDetection(void){

	if (millis() >= barrierdetect_Timer) {
		barrierdetect_Timer = millis() + BARRIERDETECT_DELAY;
		
		switch(barrierdetect_phase){
			case BARRIERDETECT_PHASE_IDLE:
				if(barrierdetect_isEnabled){
					DebugMessageBD(String(millis()) + F(" BD: IDLE. Enabled = ") + String(barrierdetect_isEnabled));
					barrierdetect_phase = BARRIERDETECT_PHASE_SERVO_POSITIONING;
				}
				else
					#ifndef SIMULATOR
					if(servo_get_position()!=BARRIERDETECT_PARK_POSITION){
						DebugMessageBD(String(millis()) + F(" BD: IDLE. Parking"));
						barrierdetect_phase = BARRIERDETECT_PHASE_SERVO_PARKING;
					}
					#endif
				break;
				
			//----------------------------------------------------				
			case BARRIERDETECT_PHASE_SERVO_POSITIONING:

				DebugMessageBD(String(millis()) + F(" BD: SERVO_POSITIONING. Pos = ") + String(barrierdetect_points[barrierdetect_pos_num].Position));

				servo_goto_position(barrierdetect_points[barrierdetect_pos_num].Position);	
				barrierdetect_phase = BARRIERDETECT_PHASE_SERVO_WAIT_STOP;
				break;
				
			case BARRIERDETECT_PHASE_SERVO_WAIT_STOP:
				if(servo_isWaiting){
					DebugMessageBD(String(millis()) + F(" BD: SERVO_WAIT_STOP. Servo waiting."));
					barrierdetect_phase = BARRIERDETECT_PHASE_SONAR_PING;
				}
				break;
								
			case BARRIERDETECT_PHASE_SONAR_PING:
				DebugMessageBD(String(millis()) + F(" BD: SONAR_PING."));
				sonar_isPingEnabled = true;			
				barrierdetect_phase = BARRIERDETECT_PHASE_SONAR_WAIT_ECHO;			
				break;
						
			case BARRIERDETECT_PHASE_SONAR_WAIT_ECHO:
				if(sonar_isEchoChecked){
					DebugMessageBD(String(millis()) + F(" BD: SONAR_WAIT_ECHO. Echo checked."));
					barrierdetect_points[barrierdetect_pos_num].Distance = sonar_ping_result;					
					if(++barrierdetect_pos_num >= barrierdetect_get_servo_positions_cnt()) barrierdetect_pos_num = 0; 
					barrierdetect_phase = BARRIERDETECT_PHASE_CALCULATION;
				}
				break;		
				
			case BARRIERDETECT_PHASE_CALCULATION:
				barrierdetect_distance = barrierdetect_calculate_distance();
				DebugMessageBD(String(millis()) + F(" BD: CALCULATION. Distance = ") + String(barrierdetect_distance));
				barrierdetect_phase = BARRIERDETECT_PHASE_REACTION;
				break;								

			case BARRIERDETECT_PHASE_REACTION:
				DebugMessageBD(String(millis()) + F(" BD: REACTION."));
				barrierdetect_motor_speed_limitation();
				barrierdetect_phase = BARRIERDETECT_PHASE_IDLE;
				break;
			
			//----------------------------------------------------------
			case BARRIERDETECT_PHASE_SERVO_PARKING:
				DebugMessageBD(String(millis()) + F(" BD: SERVO_PARKING."));
				servo_goto_position(BARRIERDETECT_PARK_POSITION);
				barrierdetect_phase = BARRIERDETECT_PHASE_SERVO_WAIT_PARKED;
				break;

			case BARRIERDETECT_PHASE_SERVO_WAIT_PARKED:
				if(servo_isWaiting){
					DebugMessageBD(String(millis()) + F(" BD: SERVO_WAIT_PARKED. Servo waiting."));
					barrierdetect_phase = BARRIERDETECT_PHASE_IDLE;
				}
				break;

								
			default:
				break;			
		}


	}
}

//==============================================================
// Нахождение минимального значения дистанции
uint8_t barrierdetect_calculate_distance(void){
	uint8_t distance = SONAR_MAX_DISTANCE;
		
	for (uint8_t i=0;i<barrierdetect_get_servo_positions_cnt();i++){
		if (barrierdetect_points[i].Distance < distance) distance = barrierdetect_points[i].Distance;
	}
	return distance;
}


//==============================================================
uint8_t barrierdetect_get_distance(){
	return barrierdetect_distance;	
}


//==============================================================
// Замедление в зависимости от расстояния до препятствия
void barrierdetect_motor_speed_limitation(void){
	
	if (barrierdetect_distance >= BARRIERDETECT_DISTANCE_MAX){
		// максимальная скорость
		motors_set_max_speed(get_motors_max_speed(), false);
	}
	else if (barrierdetect_distance < BARRIERDETECT_DISTANCE_MIN && get_motors_max_speed() > 0) {
		// остановка
		motors_set_max_speed(0, true);
	}
	else if (barrierdetect_distance < BARRIERDETECT_DISTANCE_MID && get_motors_max_speed() > BARRIERDETECT_SPEED_LOWEST) {
		// низкая скорость
		motors_set_max_speed(BARRIERDETECT_SPEED_LOWEST, true);
	}
	else if (barrierdetect_distance < BARRIERDETECT_DISTANCE_MAX && get_motors_max_speed() > BARRIERDETECT_SPEED_LOWER) {
		// пониженная скорость
		motors_set_max_speed(BARRIERDETECT_SPEED_LOWER, true);
	}
	
	DebugMessageBD(String(millis()) + motors_string_maxspeeds());
}

//==============================================================
uint8_t barrierdetect_get_servo_positions_cnt(void)
{
	return sizeof(barrierdetect_points)/sizeof(strDistanceMeas);

}

//==============================================================
void barrierdetect_enable(void){
	barrierdetect_isEnabled = true;	
}

//==============================================================
// отключаем детектор препятствий и снимаем ограничения скорости
void barrierdetect_disable(void){
	motors_set_max_speed(get_motors_max_speed(), false);
	barrierdetect_isEnabled = false;
}