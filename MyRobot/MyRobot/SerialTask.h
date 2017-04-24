/*
 * SerialHandler.h
 *
 * Created: 11.04.2017 9:39:21
 *  Author: soan1006
 */ 


#ifndef SERIALHANDLER_H_
#define SERIALHANDLER_H_

#define SERIAL_DELAY 10

extern unsigned long serial_Timer;

void serial_init(void);
void Task_SerialHandler(void);

#endif /* SERIALHANDLER_H_ */
