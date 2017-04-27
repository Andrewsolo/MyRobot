#include <Arduino.h>

#include "ServoTask.h"
#include "SerialTask.h"
#include "SonarTask.h"
#include "BarrierDetectTask.h"
#include "MotionAutomatTask.h"
#include "Motors.h"


#include <Wire.h>
#include <Adafruit_ADXL345_U.h>

//====== Глобальные переменные
/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

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

#ifndef SIMULATOR
	if(!accel.begin())
	{
		/* There was a problem detecting the ADXL345 ... check your connections */
		Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
		while(1);
	}
	accel.setRange(ADXL345_RANGE_16_G);
	
	Serial.println("X = " + String(accel.getX()));
	Serial.println("Y = " + String(accel.getY()));
	Serial.println("Z = " + String(accel.getZ()));
#endif	

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
