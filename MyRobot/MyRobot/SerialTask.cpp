/*
 * SerialHandler.cpp
 *
 * Created: 11.04.2017 9:39:06
 *  Author: soan1006
 */ 

#include <Arduino.h>
#include "SerialTask.h"
#include "BarrierDetectTask.h"
#include "Motors.h"

boolean isRCEnabled = true;
uint8_t lastCmd;

//==============================================================
void serial_init(void){
	
	Serial.begin(115200);
	Serial.println(F("Start"));	
}

//==============================================================
void Task_SerialHandler(void){
	
	if(Serial.available() > 0){
		uint8_t Cmd = Serial.read();

		// Detect arduino sketch upload sequence
		if (Cmd==' ' && lastCmd=='0'){
			//wdt_enable(WDTO_15MS); //while(1){delay(1);};
			digitalWrite(PIN_RESET, 0);
		}

		// Disable Remote Control
		else if (Cmd=='X'){		// RC on
			if (motors_get_speed()==0){
				isRCEnabled = false;
				digitalWrite(LED_BUILTIN, HIGH);
			}
		}
		// Enable Remote Control
		else if (Cmd=='x'){		// RC off
			if (motors_get_speed()==0){
				isRCEnabled = true;
				digitalWrite(LED_BUILTIN, LOW);
			}
		}


		//-------------------------------------
		if (isRCEnabled){
			// Bluetooth Remote Control commands
			if (Cmd=='F'){			// Forward command
				barrierdetect_enable();
				motors_go_forward();
			}
			else if (Cmd=='G'){		// Forward Left command
				barrierdetect_enable();
				motors_go_forward_and_left();
			}

			else if (Cmd=='I'){		// Forward Right command
				barrierdetect_enable();
				motors_go_forward_and_right();
			}

			else if (Cmd=='B'){		// Backward command
				motors_go_backward();
			}

			else if (Cmd=='H'){		// Backward Left command
				motors_go_backward_and_left();
			}

			else if (Cmd=='J'){		// Backward Right command
				motors_go_backward_and_right();
			}

			else if (Cmd=='L'){		// Left command
				motors_go_left();
			}

			else if (Cmd=='R'){		// Right command
				motors_go_right();
			}

			//else if (Cmd=='L'){
			//motors_correctspeed(-1);
			//}
			//else if (Cmd=='R'){
			//motors_correctspeed(1);
			//}

			else if (Cmd == 'S'){
				barrierdetect_disable();	// отключаем детектор препятствий и снимаем ограничения скорости
				motors_speed_down(MOTORS_STOP_STEP,MOTORS_STOP_STEP);
			}

			//-----------------------------------------
			else if (Cmd=='W'){		// Front lights on
			}
			else if (Cmd=='w'){		// Front lights off
			}

			else if (Cmd=='U'){		// Back lights on
			}
			else if (Cmd=='u'){		// Back lights off
			}

			else if (Cmd=='V'){		// Horn sound play
			}
			else if (Cmd=='v'){		// Horn sound stop
			}


			//---- команды изменения скорости ---------
			else if ((Cmd>='0' && Cmd<='9') || Cmd == 'q'){		// Speed 0/4
				uint8_t newspeed;
				if (Cmd == 'q') newspeed = 10;
				else newspeed = (uint8_t)(Cmd - '0');
				motors_set_max_speed(newspeed*(MOTOR_SPEED_MAX-MOTOR_SPEED_MIN)/10 + MOTOR_SPEED_MIN, false);
			}
			else{
				motors_speed_down(20,20);	// если обнаружена неизвестная команда, то останавливаемся, чтобы не продолжать движение без соответствующей команды.
			}

		}	//isRCEnabled

		lastCmd = Cmd;
		serial_send_debug();
	}
}

//=================================================================================
void serial_send_debug(void){
	//Serial.println(motors_get_direction_string());
	//Serial.println(motors_string_speeds());
	Serial.println(motors_string_realspeeds());
	Serial.println(motors_string_maxspeeds());
}