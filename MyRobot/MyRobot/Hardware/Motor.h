/* 
* Motor.h
*
* Created: 13.03.2017 21:54:54
* Author: ANSO
*/

#ifndef __MOTOR_H__
#define __MOTOR_H__

#include <Arduino.h>

#define MOTOR_DIR_FORWARD  true
#define MOTOR_DIR_BACKWARD false
#define MOTOR_SPEED_CORR 10
#define MOTOR_SPEED_MIN 150
#define MOTOR_SPEED_MAX (255 - MOTOR_SPEED_CORR)


class Motor
{

public:
	Motor(uint8_t pin_dir, uint8_t pin_spd);
	void set_speed(uint8_t spd);
	uint8_t get_speed(void);
	void set_max_speed(uint8_t spd);
	uint8_t get_max_speed(void);
	void set_direction(boolean dir);
	boolean get_direction(void);
	void set_speed_corr(uint8_t corr);
	uint8_t get_speed_corr(void);
	void speed_up(uint8_t step);
	void speed_down(uint8_t step);
	int get_realspeed(void);
	
private:
	uint8_t PIN_DIR;
	uint8_t PIN_SPEED;
	uint8_t Speed = 0;
	int SpeedCorrection = 0;
	uint8_t MaxSpeed = MOTOR_SPEED_MAX;
	boolean Direction = MOTOR_DIR_FORWARD;
	//boolean isStopped = true;

}; //Motor

#endif //__MOTOR_H__
