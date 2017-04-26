#include <Arduino.h>

#include "ServoTask.h"
#include "SerialTask.h"
#include "SonarTask.h"
#include "BarrierDetectTask.h"
#include "MotionAutomatTask.h"
#include "Motors.h"

//====== Глобальные переменные


//=====================================================================================================================================
void setup()
{
	//wdt_disable();

	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(PIN_RESET, OUTPUT);
	digitalWrite(PIN_RESET, 1);

	// Инициализация оборудования
	Serial_init();
	MA_init();
	BD_init();
	motors_init();
	
	// Инициализация таймеров
	MA_Timer		= millis();
	BD_Timer		= millis();
	Serial_Timer	= millis();
	Servo_Timer		= millis();
	Sonar_Timer		= millis();	
}

void loop()
{
	Serial_Task();
	MA_Task();
	BD_Task();
	Servo_Task();
	Sonar_Task();
}

//==============================================================
