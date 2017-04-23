/*
 * DebugMessage.h
 *
 * Created: 21.04.2017 13:37:02
 *  Author: soan1006
 */


#ifndef DEBUGMESSAGE_H_
#define DEBUGMESSAGE_H_

#include "Arduino.h"

#define DEBUG_MA
//#define DEBUG_BD

#ifdef DEBUG
	#define DebugMessage(Str) {Serial.println(Str);}
#else
	#define DebugMessage(Str) {}
#endif

#ifdef DEBUG_MA
	#define DebugMessageMA(Str) {Serial.println(Str);}
#else
	#define DebugMessageMA(Str) {}
#endif

#ifdef DEBUG_BD
	#define DebugMessageBD(Str) {Serial.println(Str);}
#else
	#define DebugMessageBD(Str) {}
#endif

#endif /* DEBUGMESSAGE_H_ */