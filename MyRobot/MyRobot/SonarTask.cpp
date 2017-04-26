/*
 * SonarTask.cpp
 *
 * Created: 12.04.2017 9:15:01
 *  Author: soan1006
 */

#include <Arduino.h>
#include "SonarTask.h"

// Global
#define SONAR_PING_TIMEOUT 50			// Если не получим отклик, то через это время будет отправлен еще один ping
uint32_t	Sonar_Timer;		// Holds the next ping time.

uint32_t	Sonar_ping_result = 0;
boolean		Sonar_isPingEnabled = false;
boolean		Sonar_isEchoChecked = true;


// Local
NewPing Sonar(SONAR_TRIGGER_PIN, SONAR_ECHO_PIN, SONAR_MAX_DISTANCE);
void	Sonar_echoCheck(void);

//==============================================================
void Sonar_Task(void){

	if (millis() >= Sonar_Timer && Sonar_isPingEnabled) {
		Sonar_Timer = millis() + SONAR_PING_TIMEOUT;
#ifndef SIMULATOR
		Sonar_isEchoChecked = false;
		Sonar.ping_timer(Sonar_echoCheck); // Send out the ping
#else		
		Sonar_ping_result = 20;
		Sonar_isEchoChecked = true;
		Sonar_isPingEnabled = false;
#endif

	}
}

void Sonar_echoCheck(void) { // Timer2 interrupt (every 24uS)
	if (Sonar.check_timer()) {
		// ДОРАБОТКА! В библиотеке возврат из check_timer() с true при таймауте с возвратом ping_result
		Sonar_ping_result = Sonar.ping_result / US_ROUNDTRIP_CM;
		Sonar_isEchoChecked = true;
		Sonar_isPingEnabled = false;
	}
}