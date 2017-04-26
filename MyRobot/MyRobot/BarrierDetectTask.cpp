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
uint32_t BD_Timer;

strDistanceMeas BD_points[] = {90, 0, 120, 0, 90, 0, 60, 0};
#define BARRIERDETECT_PARK_POSITION BD_points[0].Position

int		BD_distance = 0;										// TODO перенести расчет sonar_distance в модуль sonar или servo
uint8_t BD_pos_num = 0;
boolean BD_isEnabled = false;
boolean BD_isDistanceCalculated = false;

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
phases BD_phase = BARRIERDETECT_PHASE_IDLE;

uint8_t	BD_calculate_distance(void);
void	BD_motor_speed_limitation(void);
uint8_t BD_get_servo_positions_cnt(void);

//==============================================================
void BD_init(void){
	
#ifndef SIMULATOR	
	ServoV_init();
#endif	
	
	//servo_go_position(BARRIERDETECT_PARK_POSITION);
	//DebugMessageLn(String(millis()) + F(" BD started"));
	
}

//==============================================================
void BD_Task(void){

	if (millis() >= BD_Timer) {
		BD_Timer = millis() + BARRIERDETECT_DELAY;
		
		switch(BD_phase){
			case BARRIERDETECT_PHASE_IDLE:
				if(BD_isEnabled){
					DebugMessageBD(String(millis()) + F(" BD: IDLE. Enabled = ") + String(BD_isEnabled));
					BD_phase = BARRIERDETECT_PHASE_SERVO_POSITIONING;
				}
				else
					#ifndef SIMULATOR
					if(Servo_get_position()!=BARRIERDETECT_PARK_POSITION){
						DebugMessageBD(String(millis()) + F(" BD: IDLE. Parking"));
						BD_phase = BARRIERDETECT_PHASE_SERVO_PARKING;
					}
					#endif
				break;
				
			//----------------------------------------------------				
			case BARRIERDETECT_PHASE_SERVO_POSITIONING:

				DebugMessageBD(String(millis()) + F(" BD: SERVO_POSITIONING. Pos = ") + String(BD_points[BD_pos_num].Position));

				Servo_goto_position(BD_points[BD_pos_num].Position);	
				BD_phase = BARRIERDETECT_PHASE_SERVO_WAIT_STOP;
				break;
				
			case BARRIERDETECT_PHASE_SERVO_WAIT_STOP:
				if(Servo_isWaiting){
					DebugMessageBD(String(millis()) + F(" BD: SERVO_WAIT_STOP. Servo waiting."));
					BD_phase = BARRIERDETECT_PHASE_SONAR_PING;
				}
				break;
								
			case BARRIERDETECT_PHASE_SONAR_PING:
				DebugMessageBD(String(millis()) + F(" BD: SONAR_PING."));
				Sonar_isPingEnabled = true;			
				BD_phase = BARRIERDETECT_PHASE_SONAR_WAIT_ECHO;			
				break;
						
			case BARRIERDETECT_PHASE_SONAR_WAIT_ECHO:
				if(Sonar_isEchoChecked){
					DebugMessageBD(String(millis()) + F(" BD: SONAR_WAIT_ECHO. Echo checked."));
					BD_points[BD_pos_num].Distance = Sonar_ping_result;					
					if(++BD_pos_num >= BD_get_servo_positions_cnt()) BD_pos_num = 0; 
					BD_phase = BARRIERDETECT_PHASE_CALCULATION;
				}
				break;		
				
			case BARRIERDETECT_PHASE_CALCULATION:
				BD_distance = BD_calculate_distance();
				DebugMessageBD(String(millis()) + F(" BD: CALCULATION. Distance = ") + String(BD_distance));
				BD_phase = BARRIERDETECT_PHASE_REACTION;
				break;								

			case BARRIERDETECT_PHASE_REACTION:
				DebugMessageBD(String(millis()) + F(" BD: REACTION."));
				BD_motor_speed_limitation();
				BD_phase = BARRIERDETECT_PHASE_IDLE;
				break;
			
			//----------------------------------------------------------
			case BARRIERDETECT_PHASE_SERVO_PARKING:
				DebugMessageBD(String(millis()) + F(" BD: SERVO_PARKING."));
				Servo_goto_position(BARRIERDETECT_PARK_POSITION);
				BD_phase = BARRIERDETECT_PHASE_SERVO_WAIT_PARKED;
				break;

			case BARRIERDETECT_PHASE_SERVO_WAIT_PARKED:
				if(Servo_isWaiting){
					DebugMessageBD(String(millis()) + F(" BD: SERVO_WAIT_PARKED. Servo waiting."));
					BD_phase = BARRIERDETECT_PHASE_IDLE;
				}
				break;

								
			default:
				break;			
		}


	}
}

//==============================================================
// Нахождение минимального значения дистанции
uint8_t BD_calculate_distance(void){
	uint8_t distance = SONAR_MAX_DISTANCE;
		
	for (uint8_t i=0;i<BD_get_servo_positions_cnt();i++){
		if (BD_points[i].Distance < distance) distance = BD_points[i].Distance;
	}
	BD_isDistanceCalculated = true;
	return distance;
}


//==============================================================
uint8_t BD_get_distance(){
	return BD_distance;	
}


//==============================================================
// Замедление в зависимости от расстояния до препятствия
void BD_motor_speed_limitation(void){
	
	if (BD_distance >= BARRIERDETECT_DISTANCE_MAX){
		// максимальная скорость
		motors_set_max_speed(get_motors_max_speed(), false);
	}
	else if (BD_distance < BARRIERDETECT_DISTANCE_MIN && get_motors_max_speed() > 0) {
		// остановка
		motors_set_max_speed(0, true);
	}
	else if (BD_distance < BARRIERDETECT_DISTANCE_MID && get_motors_max_speed() > BARRIERDETECT_SPEED_LOWEST) {
		// низкая скорость
		motors_set_max_speed(BARRIERDETECT_SPEED_LOWEST, true);
	}
	else if (BD_distance < BARRIERDETECT_DISTANCE_MAX && get_motors_max_speed() > BARRIERDETECT_SPEED_LOWER) {
		// пониженная скорость
		motors_set_max_speed(BARRIERDETECT_SPEED_LOWER, true);
	}
	
	DebugMessageBD(String(millis()) + motors_string_maxspeeds());
}

//==============================================================
uint8_t BD_get_servo_positions_cnt(void)
{
	return sizeof(BD_points)/sizeof(strDistanceMeas);

}

//==============================================================
void BD_enable(void){
	BD_isEnabled = true;	
}

//==============================================================
// отключаем детектор препятствий и снимаем ограничения скорости
void BD_disable(void){
	motors_set_max_speed(get_motors_max_speed(), false);
	
	for (uint8_t i=0;i<BD_get_servo_positions_cnt();i++)
		BD_points[i].Distance = 0;
	BD_isDistanceCalculated = false;
	BD_pos_num = 0;
	BD_isEnabled = false;
}