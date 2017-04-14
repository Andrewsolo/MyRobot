#include <Arduino.h>

#include "Motors.h"
#include "ServoTask.h"
#include "SerialTask.h"
#include "SonarTask.h"
#include "BarrierDetectTask.h"

//====== Глобальные переменные






//=====================================================================================================================================
void setup()
{
	//wdt_disable();

	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(PIN_RESET, OUTPUT);
	digitalWrite(PIN_RESET, 1);

	// Инициализация оборудования
	serial_init();
	motors_correctspeed(-3);
	//servo_go_park();
	/*
	servo_h.attach(SERVO_H_PIN);
	servo_h.write(servo_h_positions[servo_h_position] + SERVO_H_POS_CORRECTION);
	delay(200);
	servo_h.detach();
	isServoPositioned = true;
	*/

	/*
	servo_v.attach(SERVO_V_PIN);
	servo_v.write(60);		// 100 - max?
	delay(200);
	servo_v.detach();
	*/

	// Инициализация таймеров
	sonar_pingTimer = millis();
	servo_h_rotationTimer = millis();
	barrierdetect_Timer = millis();
}

void loop()
{
	Task_SerialHandler();
	Task_ServoHandler();
	Task_SonarHandler();
	Task_BarrierDetection();
}

//==============================================================
