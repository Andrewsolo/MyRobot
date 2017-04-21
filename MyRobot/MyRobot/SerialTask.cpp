/*
 * SerialHandler.cpp
 *
 * Created: 11.04.2017 9:39:06
 *  Author: soan1006
 */ 

#include <Arduino.h>
#include "SerialTask.h"
#include "BarrierDetectTask.h"
#include "MotionAutomatTask.h"
#include "Motors.h"
#include "DebugMessage.h"

boolean isRCEnabled = true;
uint8_t lastCmd;

//==============================================================
void serial_init(void){
	
	Serial.begin(115200L);
	DebugMessageLn(F("Serial started"));	

}

//==============================================================
void Task_SerialHandler(void){
	
	if(1 || Serial.available() > 0){	//DEBUG

		uint8_t cmd = Serial.read();

		// Detect arduino sketch upload sequence
		if (cmd==' ' && lastCmd=='0'){
			//wdt_enable(WDTO_15MS); //while(1){delay(1);};
			digitalWrite(PIN_RESET, 0);
		}

		// Disable Remote Control
		else if (cmd=='X'){		// RC on
			if (motors_get_speed()==0){
				isRCEnabled = false;
				digitalWrite(LED_BUILTIN, HIGH);
			}
		}
		// Enable Remote Control
		else if (cmd=='x'){		// RC off
			if (motors_get_speed()==0){
				isRCEnabled = true;
				digitalWrite(LED_BUILTIN, LOW);
			}
		}


		//-------------------------------------
		if (isRCEnabled){
			// Bluetooth Remote Control commands
			if (cmd=='F'){			// Forward command
				motionautomat_add_command(MA_COMMAND_FORWARD);
			}
			else if (cmd=='G'){		// Forward Left command
				motionautomat_add_command(MA_COMMAND_FORWARD_AND_LEFT);
			}

			else if (cmd=='I'){		// Forward Right command
				motionautomat_add_command(MA_COMMAND_FORWARD_AND_RIGHT);
			}

			else if (cmd=='B'){		// Backward command
				motionautomat_add_command(MA_COMMAND_BACKWARD);
			}

			else if (cmd=='H'){		// Backward Left command
				motionautomat_add_command(MA_COMMAND_BACKWARD_AND_LEFT);
			}

			else if (cmd=='J'){		// Backward Right command
				motionautomat_add_command(MA_COMMAND_BACKWARD_AND_RIGHT);
			}

			else if (cmd=='L'){		// Left command
				motionautomat_add_command(MA_COMMAND_LEFT);
			}

			else if (cmd=='R'){		// Right command
				motionautomat_add_command(MA_COMMAND_RIGHT);
			}

			//else if (cmd=='L'){
			//motors_correctspeed(-1);
			//}
			//else if (cmd=='R'){
			//motors_correctspeed(1);
			//}

			else if (cmd == 'S'){
				motionautomat_add_command(MA_COMMAND_STOP);
			}

			//-----------------------------------------
			else if (cmd=='W'){		// Front lights on
			}
			else if (cmd=='w'){		// Front lights off
			}

			else if (cmd=='U'){		// Back lights on
			}
			else if (cmd=='u'){		// Back lights off
			}

			else if (cmd=='V'){		// Horn sound play
			}
			else if (cmd=='v'){		// Horn sound stop
			}

			//---- команды изменения скорости ---------
			else if ((cmd>='0' && cmd<='9') || cmd == 'q'){		// Speed 0/4
				uint8_t newspeed;
				if (cmd == 'q') newspeed = 10;
				else newspeed = (uint8_t)(cmd - '0');
				motionautomat_add_command((MA_commands_enum)((uint8_t)MA_COMMAND_SPEED_0 + newspeed));
			}
			else{
				//DEBUG motionautomat_add_command(MA_COMMAND_STOP);	// если обнаружена неизвестная команда, то останавливаемся, чтобы не продолжать движение без соответствующей команды.
			}

		}	//isRCEnabled

		lastCmd = cmd;
		serial_send_debug();
	}
}

//=================================================================================
void serial_send_debug(void){
	//DebugMessageLn(motors_get_direction_string());
	//DebugMessageLn(motors_string_speeds());
	DebugMessageLn(motors_string_realspeeds());
	DebugMessageLn(motors_string_maxspeeds());
}