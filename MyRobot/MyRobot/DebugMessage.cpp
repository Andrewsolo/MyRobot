#include "DebugMessage.h"

void DebugMessage(String Str){
	#ifdef DEBUG
	Serial.print(Str);
	#endif
}

void DebugMessageLn(String Str){
	#ifdef DEBUG
	Serial.println(Str);
	#endif
}
