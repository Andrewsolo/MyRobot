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
	serial_init();
	motionautomat_init();
	barrierdetect_init();

	motors_correctspeed(-3);
	
	//delay(1000);
	


	// Инициализация таймеров
	motionautomat_Timer = millis();
	barrierdetect_Timer = millis();
	servo_h_rotationTimer = millis();
	sonar_pingTimer = millis();	
}

void loop()
{
	Task_SerialHandler();
	Task_MotionAutomat();
	Task_BarrierDetection();
	Task_ServoHandler();
	Task_SonarHandler();
}

//==============================================================
