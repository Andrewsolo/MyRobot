/*
 * SonarTask.cpp
 *
 * Created: 12.04.2017 9:15:01
 *  Author: soan1006
 */

#include <Arduino.h>
#include "SonarTask.h"

// Global
#define SONAR_PING_TIMEOUT 50			// ���� �� ������� ������, �� ����� ��� ����� ����� ��������� ��� ���� ping
unsigned long	sonar_pingTimer;		// Holds the next ping time.

unsigned long	sonar_ping_result = 0;
boolean			sonar_isPingEnabled = false;
boolean			sonar_isEchoChecked = true;


// Local
NewPing sonar(SONAR_TRIGGER_PIN, SONAR_ECHO_PIN, SONAR_MAX_DISTANCE);
void	sonar_echoCheck(void);

//==============================================================
void Task_SonarHandler(void){

	if (millis() >= sonar_pingTimer && sonar_isPingEnabled) {
		sonar_pingTimer = millis() + SONAR_PING_TIMEOUT;
#ifndef SIMULATOR
		sonar_isEchoChecked = false;
		sonar.ping_timer(sonar_echoCheck); // Send out the ping
#else		
		sonar_ping_result = 20;
		sonar_isEchoChecked = true;
		sonar_isPingEnabled = false;
#endif

	}
}

void sonar_echoCheck(void) { // Timer2 interrupt (every 24uS)
	if (sonar.check_timer()) {
		// ���������! � ���������� ������� �� check_timer() � true ��� �������� � ��������� ping_result
		sonar_ping_result = sonar.ping_result / US_ROUNDTRIP_CM;
		sonar_isEchoChecked = true;
		sonar_isPingEnabled = false;
	}
}