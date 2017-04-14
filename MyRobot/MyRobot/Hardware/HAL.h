/*
 * HAL.h
 *
 * Created: 09.04.2017 18:48:36
 *  Author: ANSO
 */


#ifndef HAL_H_
#define HAL_H_


#define MOTOR_L_DIR_PIN	4	// IN1
#define MOTOR_L_SPD_PIN	5	// ENA (pwm)
#define MOTOR_R_DIR_PIN	7	// IN3
#define MOTOR_R_SPD_PIN	6	// ENB (pwm)

// Так как библиотека Servo занимает таймер1, то PWM на выходах 9 и 10 использовать не удастся. Поэтому подключим на них серводвигатели
#define SERVO_H_PIN	9
#define SERVO_V_PIN	10

// Библиотека NewPing (при работе с таймером) делает невозможным работу PWM на 3 и 11 выводах
#define SONAR_TRIGGER_PIN   A2
#define SONAR_ECHO_PIN      A1

//======= OTHERS =============================================
#define PIN_RESET 14	//A0


#endif /* HAL_H_ */