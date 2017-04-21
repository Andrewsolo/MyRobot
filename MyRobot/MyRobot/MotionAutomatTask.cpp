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

#define MA_COMMANDS_BUFFER_SIZE 5
MA_commands_enum MA_commands[MA_COMMANDS_BUFFER_SIZE];
uint8_t MA_commands_head;
uint8_t MA_commands_tail;

#define MOTIONAUTOMAT_DELAY	50
unsigned long motionautomat_Timer;


MA_commands_enum motionautomat_get_command(void);

//==============================================================
void motionautomat_init(void){
	MA_commands_head = 0;
	MA_commands_tail = MA_commands_head;

	DebugMessage(String(millis()) + F(" MA started"));
}

//==============================================================
void Task_MotionAutomat(void){
	
	if (millis() >= motionautomat_Timer) {
		motionautomat_Timer += MOTIONAUTOMAT_DELAY;
		
		if(MA_commands_head != MA_commands_tail){	// ≈сли в буфере есть команды
			
			MA_commands_enum cmd = motionautomat_get_command();

			DebugMessage(String(millis()) + F(" MA command = ") + String((uint8_t)cmd));
			
			if(cmd == MA_COMMAND_FORWARD){
				barrierdetect_enable();
				motors_go_forward();				
			}
			else if(cmd == MA_COMMAND_FORWARD_AND_LEFT){
				barrierdetect_enable();
				motors_go_forward_and_left();
			}
			else if(cmd == MA_COMMAND_FORWARD_AND_RIGHT){
				barrierdetect_enable();
				motors_go_forward_and_right();
			}
			else if(cmd == MA_COMMAND_BACKWARD){
				motors_go_backward();
			}
			else if(cmd == MA_COMMAND_BACKWARD_AND_LEFT){
				motors_go_backward_and_left();
			}
			else if(cmd == MA_COMMAND_BACKWARD_AND_RIGHT){
				motors_go_backward_and_right();
			}
			else if(cmd == MA_COMMAND_LEFT){
				motors_go_left();
			}
			else if(cmd == MA_COMMAND_RIGHT){
				motors_go_right();
			}
			else if(cmd == MA_COMMAND_STOP){
				barrierdetect_disable();	// отключаем детектор преп€тствий и снимаем ограничени€ скорости
				motors_speed_down(MOTORS_STOP_STEP,MOTORS_STOP_STEP);
			}
			else if(cmd>=MA_COMMAND_SPEED_0 && cmd<=MA_COMMAND_SPEED_10){					
				uint8_t newspeed;
				newspeed = (uint8_t)(cmd - (uint8_t)MA_COMMAND_SPEED_0);
				motors_set_max_speed(newspeed*(MOTOR_SPEED_MAX-MOTOR_SPEED_MIN)/10 + MOTOR_SPEED_MIN, false);
			}
			else if(cmd == MA_COMMAND_IDLE){}
		}
	
	}
}

//==============================================================
// ƒобавление команды в конец очереди
void motionautomat_add_command(MA_commands_enum CMD){
	
	MA_commands[MA_commands_tail++] = CMD;
	if(MA_commands_tail == sizeof(MA_commands)) MA_commands_tail = 0;
	if(MA_commands_tail == MA_commands_head){
		// затираем команду, которую не успели обработать
		if(MA_commands_head++ == sizeof(MA_commands)) MA_commands_head = 0;
	}
}

//==============================================================
// »звлечение команды из начала очереди
MA_commands_enum motionautomat_get_command(void){
	MA_commands_enum cmd = MA_COMMAND_IDLE;
	if(MA_commands_head != MA_commands_tail){	// ≈сли в буфере есть команды
		cmd = MA_commands[MA_commands_head];
		MA_commands[MA_commands_head] = MA_COMMAND_IDLE;	// Ќеоб€зательно, так как окажетс€ вне очереди. ƒл€ отладки
		if(++MA_commands_head == sizeof(MA_commands)) MA_commands_head = 0;
	}
	return cmd;
}