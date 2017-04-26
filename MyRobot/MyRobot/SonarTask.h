/*
 * SonarTask.h
 *
 * Created: 12.04.2017 9:15:10
 *  Author: soan1006
 */ 


#ifndef SONARTASK_H_
#define SONARTASK_H_

#include "NewPing.h"
#include "HAL.h"

//====== SONAR (SR04) ========================================
#if (!defined (SONAR_TRIGGER_PIN)) || (!defined SONAR_ECHO_PIN)
#error "SONAR Pins not defined!"
#endif

#define SONAR_MAX_DISTANCE 200	// Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

extern uint32_t Sonar_Timer;
extern uint32_t Sonar_ping_result;
extern boolean Sonar_isEchoChecked;
extern boolean Sonar_isPingEnabled;

void Sonar_Task(void);


#endif /* SONARTASK_H_ */