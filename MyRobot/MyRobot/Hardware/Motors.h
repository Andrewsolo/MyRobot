/*
 * Motors.h
 *
 * Created: 08.04.2017 21:32:55
 *  Author: ANSO
 *
 *  MOTOR (Motor Shield)
 */

#ifndef MOTORS_H_
#define MOTORS_H_

//#include "Arduino.h"
#include "Motor.h"
#include "HAL.h"

#if (!defined (MOTOR_L_DIR_PIN)) || (!defined MOTOR_L_SPD_PIN) || (!defined MOTOR_R_DIR_PIN) || (!defined MOTOR_R_SPD_PIN)
	#error "MOTORS Pins not defined!"
#endif
#define MOTORS_STOP_STEP 20
#define MOTORS_ACCEL_STEP 5
// Макросы
#define is_motors_in_right_direction(r_dir,l_dir) ((motor_right.get_direction() == r_dir) && (motor_left.get_direction() == l_dir)) || (motors_get_speed()==0)
#define motors_set_direction(r_dir,l_dir) {motor_right.set_direction(r_dir);motor_left.set_direction(l_dir);}

void	motors_speed_up(uint8_t, uint8_t);
void	motors_speed_down(uint8_t, uint8_t);
uint8_t motors_get_speed(void);
void	motors_set_max_speed(uint8_t, boolean);
uint8_t get_motors_max_speed(void);
void	motors_correctspeed(int);
// Команды движения
void	motors_go_forward(void);
void	motors_go_forward_and_left(void);
void	motors_go_forward_and_right(void);
void	motors_go_backward(void);
void	motors_go_backward_and_left(void);
void	motors_go_backward_and_right(void);
void	motors_go_left(void);
void	motors_go_right(void);
// Вывод отладочной информации
String	motors_string_direction(void);
String	motors_string_speeds(void);
String	motors_string_realspeeds(void);
String	motors_string_maxspeeds(void);
#endif /* MOTORS_H_ */