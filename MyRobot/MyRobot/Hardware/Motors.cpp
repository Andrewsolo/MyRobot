/*
 * Motors.cpp
 *
 * Created: 08.04.2017 21:32:31
 *  Author: ANSO
 */

#include "Arduino.h"
#include "Motors.h"

Motor motor_right(MOTOR_R_DIR_PIN, MOTOR_R_SPD_PIN);
Motor motor_left(MOTOR_L_DIR_PIN, MOTOR_L_SPD_PIN);
uint8_t motors_max_speed = MOTOR_SPEED_MAX;

//========================================================
void motors_speed_up(uint8_t step_r, uint8_t step_l){
	uint8_t right_speed = motor_right.get_speed();
	uint8_t left_speed = motor_left.get_speed();
	if (right_speed == left_speed){
		motor_right.speed_up(step_r);
		motor_left.speed_up(step_l);
	}
	else if (right_speed > left_speed) motor_left.speed_up(right_speed-left_speed);
	else motor_right.speed_up(left_speed-right_speed);
}

//========================================================
void motors_speed_down(uint8_t step_r, uint8_t step_l){
	uint8_t right_speed = motor_right.get_speed();
	uint8_t left_speed = motor_left.get_speed();
	if (right_speed == left_speed){
		motor_right.speed_down(step_r);
		motor_left.speed_down(step_l);
	}
	else if (right_speed > left_speed) motor_right.speed_down(right_speed-left_speed);
	else motor_left.speed_down(left_speed-right_speed);
}

//========================================================
uint8_t motors_get_speed(void){
	uint8_t right_speed = motor_right.get_speed();
	uint8_t left_speed = motor_left.get_speed();

	if (right_speed > left_speed) return right_speed;
	else return left_speed;
}

//========================================================
void motors_set_max_speed(uint8_t spd, boolean temporary=false)
{
	if (!temporary) motors_max_speed = spd;
	motor_right.set_max_speed(spd);
	motor_left.set_max_speed(spd);
}

//========================================================
uint8_t get_motors_max_speed(void)
{
	return motors_max_speed;
}

//========================================================
void motors_correctspeed(int corr)
{
	if (corr==0) return;

	int l_corr = motor_left.get_speed_corr();
	int r_corr = motor_right.get_speed_corr();
	if (corr<0){	// замедлим левый мотор или ускорим правый
		corr = -corr;
		if (l_corr>0){
			l_corr -= corr;
			if (l_corr < 0){
				l_corr = 0;
				r_corr = -l_corr;
			}
		}
		else{
			r_corr += corr;
		}
	}
	else{			// замедлим правый мотор или ускорим левый
		if (r_corr>0){
			r_corr -= corr;
			if (r_corr < 0){
				r_corr = 0;
				l_corr = -r_corr;
			}
		}
		else{
			l_corr += corr;
		}
	}
	motor_left.set_speed_corr(l_corr);
	motor_right.set_speed_corr(r_corr);
}

//========================================================
void motors_go_forward(void){
	if (is_motors_in_right_direction(MOTOR_DIR_FORWARD, MOTOR_DIR_FORWARD)){
		motors_set_direction(MOTOR_DIR_FORWARD,MOTOR_DIR_FORWARD);
		motors_speed_up(MOTORS_ACCEL_STEP,MOTORS_ACCEL_STEP);
	}
	else
		motors_speed_down(MOTORS_STOP_STEP,MOTORS_STOP_STEP);
}

//========================================================
void motors_go_forward_and_left(void){
	if (is_motors_in_right_direction(MOTOR_DIR_FORWARD, MOTOR_DIR_FORWARD)){
		motors_set_direction(MOTOR_DIR_FORWARD,MOTOR_DIR_FORWARD);
		motor_right.speed_up(MOTORS_ACCEL_STEP);
		motor_left.speed_down(MOTORS_ACCEL_STEP);
	}
	else
		motors_speed_down(MOTORS_STOP_STEP,MOTORS_STOP_STEP);
}

//========================================================
void motors_go_forward_and_right(void){
	if (is_motors_in_right_direction(MOTOR_DIR_FORWARD, MOTOR_DIR_FORWARD)){
		motors_set_direction(MOTOR_DIR_FORWARD,MOTOR_DIR_FORWARD);
		motor_right.speed_down(MOTORS_ACCEL_STEP);
		motor_left.speed_up(MOTORS_ACCEL_STEP);
	}
	else
		motors_speed_down(MOTORS_STOP_STEP,MOTORS_STOP_STEP);
}

//========================================================
void motors_go_backward(void){
	if (is_motors_in_right_direction(MOTOR_DIR_BACKWARD, MOTOR_DIR_BACKWARD)){
		motors_set_direction(MOTOR_DIR_BACKWARD,MOTOR_DIR_BACKWARD);
		motors_speed_up(MOTORS_ACCEL_STEP,MOTORS_ACCEL_STEP);
	}
	else
		motors_speed_down(MOTORS_STOP_STEP,MOTORS_STOP_STEP);
}

//========================================================
void motors_go_backward_and_left(void){
	if (is_motors_in_right_direction(MOTOR_DIR_BACKWARD, MOTOR_DIR_BACKWARD)){
		motors_set_direction(MOTOR_DIR_BACKWARD,MOTOR_DIR_BACKWARD);
		motor_right.speed_down(MOTORS_ACCEL_STEP);
		motor_left.speed_up(MOTORS_ACCEL_STEP);
	}
	else
		motors_speed_down(MOTORS_STOP_STEP,MOTORS_STOP_STEP);
}

//========================================================
void motors_go_backward_and_right(void){
	if (is_motors_in_right_direction(MOTOR_DIR_BACKWARD, MOTOR_DIR_BACKWARD)){
		motors_set_direction(MOTOR_DIR_BACKWARD,MOTOR_DIR_BACKWARD);
		motor_right.speed_up(MOTORS_ACCEL_STEP);
		motor_left.speed_down(MOTORS_ACCEL_STEP);
	}
	else
		motors_speed_down(MOTORS_STOP_STEP,MOTORS_STOP_STEP);
}

//========================================================
void motors_go_left(void){
	if (is_motors_in_right_direction(MOTOR_DIR_FORWARD, MOTOR_DIR_BACKWARD)){
		motors_set_direction(MOTOR_DIR_FORWARD,MOTOR_DIR_BACKWARD);
		motor_right.speed_up(MOTORS_ACCEL_STEP);
		motor_left.speed_up(MOTORS_ACCEL_STEP);
	}
	else
		motors_speed_down(MOTORS_STOP_STEP,MOTORS_STOP_STEP);
}

//========================================================
void motors_go_right(void){
	if (is_motors_in_right_direction(MOTOR_DIR_BACKWARD, MOTOR_DIR_FORWARD)){
		motors_set_direction(MOTOR_DIR_BACKWARD,MOTOR_DIR_FORWARD);
		motor_right.speed_up(MOTORS_ACCEL_STEP);
		motor_left.speed_up(MOTORS_ACCEL_STEP);
	}
	else
		motors_speed_down(MOTORS_STOP_STEP,MOTORS_STOP_STEP);
}

//========================================================
String motors_string_direction(void){
	String Str = F("Directions: ");
	Str += String(motor_right.get_direction()) + F(" ") + String(motor_left.get_direction());
	return Str;
}

//========================================================
String motors_string_speeds(void){
	String Str = F(" Speeds: ");
	Str += String(motor_right.get_speed()) + F(" ") + String(motor_left.get_speed());
	return Str;
}

//========================================================
String motors_string_realspeeds(void){
	String Str = F(" RealSpeeds: ");
	Str += String(motor_right.get_realspeed()) + F(" ") + String(motor_left.get_realspeed());
	return Str;
}

//========================================================
String motors_string_maxspeeds(void){
	String Str = F(" Max Speeds: ");
	Str += String(motor_right.get_max_speed()) + F(" ") + String(motor_left.get_max_speed());
	return Str;
}