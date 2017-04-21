/*
 * DebugMessage.h
 *
 * Created: 21.04.2017 13:37:02
 *  Author: soan1006
 */ 


#ifndef DEBUGMESSAGE_H_
#define DEBUGMESSAGE_H_

#include "Arduino.h"

#ifdef DEBUG
	#define DebugMessage(Str) {Serial.println(String(millis()) + " " + Str);}
#else
	#define DebugMessage(Str) {}
#endif	

#endif /* DEBUGMESSAGE_H_ */