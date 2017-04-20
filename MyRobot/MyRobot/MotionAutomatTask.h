/*
 * MotionAutomatTask.h
 *
 * Created: 20.04.2017 11:56:01
 *  Author: soan1006
 */ 


#ifndef MOTIONAUTOMATTASK_H_
#define MOTIONAUTOMATTASK_H_

enum MA_commands_enum{
	MA_COMMAND_IDLE = 0,
	MA_COMMAND_FORWARD,
	MA_COMMAND_FORWARD_AND_LEFT,
	MA_COMMAND_FORWARD_AND_RIGHT,
	MA_COMMAND_BACKWARD,
	MA_COMMAND_BACKWARD_AND_LEFT,
	MA_COMMAND_BACKWARD_AND_RIGHT,
	MA_COMMAND_LEFT,
	MA_COMMAND_RIGHT,
	MA_COMMAND_STOP,
	// ������� ��������� �������� ������ ���� �� �������
	MA_COMMAND_SPEED_0,
	MA_COMMAND_SPEED_1,
	MA_COMMAND_SPEED_2,
	MA_COMMAND_SPEED_3,
	MA_COMMAND_SPEED_4,
	MA_COMMAND_SPEED_5,
	MA_COMMAND_SPEED_6,
	MA_COMMAND_SPEED_7,
	MA_COMMAND_SPEED_8,
	MA_COMMAND_SPEED_9,
	MA_COMMAND_SPEED_10};

extern unsigned long motionautomat_Timer;

void motionautomat_init(void);
void Task_MotionAutomat(void);
void motionautomat_add_command(MA_commands_enum);

#endif /* MOTIONAUTOMATTASK_H_ */