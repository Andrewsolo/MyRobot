/* 
* Motor.cpp
*
* Created: 13.03.2017 21:54:54
* Author: ANSO
*/

#include <Arduino.h>

#include "Motor.h"


Motor::Motor(uint8_t pin_dir, uint8_t pin_spd)
{
	PIN_DIR = pin_dir;
	PIN_SPEED = pin_spd;
	
	pinMode (PIN_DIR, OUTPUT);
	pinMode (PIN_SPEED, OUTPUT);
	set_direction(MOTOR_DIR_FORWARD);
	set_speed(0);
} //Motor

void Motor::set_speed(uint8_t spd)
{
	Speed = spd;
	analogWrite(PIN_SPEED, get_realspeed());
}

uint8_t Motor::get_speed(void){
	return Speed;	
}

void Motor::set_max_speed(uint8_t spd){
	MaxSpeed = spd;
	if (Speed > MaxSpeed) set_speed(MaxSpeed);
}

uint8_t Motor::get_max_speed(void){
	return MaxSpeed;
}

void Motor::set_direction(boolean dir)
{
	Direction = dir;
	if (Direction==MOTOR_DIR_FORWARD)	
		digitalWrite (PIN_DIR, HIGH);
	else
		digitalWrite (PIN_DIR, LOW);
}

boolean Motor::get_direction(void){
	return Direction;
}

void Motor::set_speed_corr(uint8_t corr)
{
	if (corr > MOTOR_SPEED_CORR) corr = MOTOR_SPEED_CORR;
	SpeedCorrection = corr;
	set_speed(get_speed());		// применим корректировку к текущей скорости
}

uint8_t Motor::get_speed_corr(void)
{
	return SpeedCorrection;
}

void Motor::speed_up(uint8_t step){
	if (Speed == 0 && MaxSpeed != 0) set_speed(MOTOR_SPEED_MIN);
	else if (Speed + step <= MaxSpeed) set_speed(Speed+step);
}

void Motor::speed_down(uint8_t step){
	if (Speed != 0){
		if (Speed-step < MOTOR_SPEED_MIN) set_speed(0);
		else set_speed(Speed-step);
	}
}
	
int Motor::get_realspeed(void){	
	if (Speed == 0) return 0;
	else return Speed + get_speed_corr();
}
