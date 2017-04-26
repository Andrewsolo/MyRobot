/*
 * MotionAutomatTask.cpp
 *
 * Created: 20.04.2017 10:17:22
 *  Author: soan1006
 *
 * Ёто некий исполнительный механизм с входным буфером дл€ кодов команд
 * ќбрабатывает по одной команде за итерацию
 *  оманды выполн€ютс€ по принципу очереди.
 * «аполнение очереди с удалением старых, если автомат не успевает обрабатывать.
 *
 */

#include <Arduino.h>
#include "MotionAutomatTask.h"
#include "BarrierDetectTask.h"
#include "Motors.h"
#include "DebugMessage.h"
#include <stdint-gcc.h>

#define MA_COMMANDS_BUFFER_SIZE 5
MA_commands_enum MA_commands[MA_COMMANDS_BUFFER_SIZE];
uint8_t MA_commands_head;
uint8_t MA_commands_tail;

#define MOTIONAUTOMAT_DELAY	30	//50 —делаем обработку быстрее, чем приход команд по bluetooth (50 мс)
uint32_t MA_Timer;

MA_commands_enum MA_get_command(void);

//==============================================================
void MA_init(void){
	MA_commands_head = 0;
	MA_commands_tail = MA_commands_head;

	DebugMessageMA(String(millis()) + F(" MA started"));
}

//==============================================================
void MA_Task(void){

	if (millis() >= MA_Timer) {
		MA_Timer = millis() + MOTIONAUTOMAT_DELAY;

		if(MA_commands_head != MA_commands_tail){	// ≈сли в буфере есть команды

			MA_commands_enum cmd = MA_get_command();

			if(cmd == MA_COMMAND_FORWARD){
				BD_enable();
				if (BD_isDistanceCalculated)
					motors_go_forward();
			}
			else if(cmd == MA_COMMAND_FORWARD_AND_LEFT){
				BD_enable();
				motors_go_forward_and_left();
			}
			else if(cmd == MA_COMMAND_FORWARD_AND_RIGHT){
				BD_enable();
				motors_go_forward_and_right();
			}
			else if(cmd == MA_COMMAND_BACKWARD){
				BD_disable();
				motors_go_backward();
			}
			else if(cmd == MA_COMMAND_BACKWARD_AND_LEFT){
				BD_disable();
				motors_go_backward_and_left();
			}
			else if(cmd == MA_COMMAND_BACKWARD_AND_RIGHT){
				BD_disable();
				motors_go_backward_and_right();
			}
			else if(cmd == MA_COMMAND_LEFT){
				BD_disable();
				motors_go_left();
			}
			else if(cmd == MA_COMMAND_RIGHT){
				BD_disable();
				motors_go_right();
			}
			else if(cmd == MA_COMMAND_STOP){
				BD_disable();
				motors_speed_down(MOTORS_STOP_STEP,MOTORS_STOP_STEP);
			}
			else if(cmd>=MA_COMMAND_SPEED_0 && cmd<=MA_COMMAND_SPEED_10){
				uint8_t newspeed;
				newspeed = (uint8_t)(cmd - (uint8_t)MA_COMMAND_SPEED_0);
				motors_set_max_speed(newspeed*(MOTOR_SPEED_MAX-MOTOR_SPEED_MIN)/10 + MOTOR_SPEED_MIN, false);
			}
			else if(cmd == MA_COMMAND_IDLE){}
				
			DebugMessageMA(String(millis()) + motors_string_realspeeds());	
		}

	}
}

//==============================================================
// ƒобавление команды в конец очереди
void MA_add_command(MA_commands_enum cmd){

	DebugMessageMA(String(millis()) + F(" MA: add cmd = ") + String((uint8_t)cmd));

	MA_commands[MA_commands_tail++] = cmd;
	if(MA_commands_tail == sizeof(MA_commands)) MA_commands_tail = 0;
	if(MA_commands_tail == MA_commands_head){
		// затираем команду, которую не успели обработать
		if(++MA_commands_head == sizeof(MA_commands)) MA_commands_head = 0;
	}
}

//==============================================================
// »звлечение команды из начала очереди
MA_commands_enum MA_get_command(void){
	MA_commands_enum cmd = MA_COMMAND_IDLE;
	if(MA_commands_head != MA_commands_tail){	// ≈сли в буфере есть команды
		cmd = MA_commands[MA_commands_head];
		MA_commands[MA_commands_head] = MA_COMMAND_IDLE;	// Ќеоб€зательно, так как окажетс€ вне очереди. ƒл€ отладки
		if(++MA_commands_head == sizeof(MA_commands)) MA_commands_head = 0;
	}
	
	DebugMessageMA(String(millis()) + F(" MA: get cmd = ") + String((uint8_t)cmd));
	return cmd;
}