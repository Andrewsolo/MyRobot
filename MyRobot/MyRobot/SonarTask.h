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
#define SONAR_PING_DELAY 50		// How frequently are we going to send out a ping (in milliseconds). 50ms would be 20 times a second.

extern unsigned long sonar_pingTimer;
extern boolean isSonarEchoChecked;
extern boolean isSonarStartMeasure;

void Task_SonarHandler(void);


#endif /* SONARTASK_H_ */