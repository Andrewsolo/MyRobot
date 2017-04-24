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

extern unsigned long sonar_pingTimer;
extern unsigned long sonar_ping_result;
extern boolean sonar_isEchoChecked;
extern boolean sonar_isPingEnabled;

void Task_SonarHandler(void);


#endif /* SONARTASK_H_ */