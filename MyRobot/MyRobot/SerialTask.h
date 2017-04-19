/*
 * SerialHandler.h
 *
 * Created: 11.04.2017 9:39:21
 *  Author: soan1006
 */ 


#ifndef SERIALHANDLER_H_
#define SERIALHANDLER_H_

void serial_init(void);
void Task_SerialHandler(void);
void serial_send_debug(void);

#endif /* SERIALHANDLER_H_ */
