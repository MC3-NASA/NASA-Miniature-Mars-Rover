#ifndef _Drive_h
#define _Drive_h

#include "arduino.h"
#include "MultiSerial.h"

/* Macro functions */
#define GET_LOW_BYTE(A) (uint8_t)((A))								//Macro function  get lower 8 bits of A

#define GET_HIGH_BYTE(A) (uint8_t)((A) >> 8)						//Macro function  get higher 8 bits of A

#define BYTE_TO_HW(A, B) ((((uint16_t)(A)) << 8) | (uint8_t)(B))	//Macro Function  put A as higher 8 bits   B as lower 8 bits   which amalgamated into 16 bits integer

/* Servo command Macros */
#define LOBOT_SERVO_FRAME_HEADER         0x55
#define LOBOT_SERVO_MOVE_TIME_WRITE      1
#define LOBOT_SERVO_MOVE_TIME_READ       2
#define LOBOT_SERVO_MOVE_TIME_WAIT_WRITE 7
#define LOBOT_SERVO_MOVE_TIME_WAIT_READ  8
#define LOBOT_SERVO_MOVE_START           11
#define LOBOT_SERVO_MOVE_STOP            12
#define LOBOT_SERVO_ID_WRITE             13
#define LOBOT_SERVO_ID_READ              14
#define LOBOT_SERVO_ANGLE_OFFSET_ADJUST  17
#define LOBOT_SERVO_ANGLE_OFFSET_WRITE   18
#define LOBOT_SERVO_ANGLE_OFFSET_READ    19
#define LOBOT_SERVO_ANGLE_LIMIT_WRITE    20
#define LOBOT_SERVO_ANGLE_LIMIT_READ     21
#define LOBOT_SERVO_VIN_LIMIT_WRITE      22
#define LOBOT_SERVO_VIN_LIMIT_READ       23
#define LOBOT_SERVO_TEMP_MAX_LIMIT_WRITE 24
#define LOBOT_SERVO_TEMP_MAX_LIMIT_READ  25
#define LOBOT_SERVO_TEMP_READ            26
#define LOBOT_SERVO_VIN_READ             27
#define LOBOT_SERVO_POS_READ             28
#define LOBOT_SERVO_OR_MOTOR_MODE_WRITE  29
#define LOBOT_SERVO_OR_MOTOR_MODE_READ   30
#define LOBOT_SERVO_LOAD_OR_UNLOAD_WRITE 31
#define LOBOT_SERVO_LOAD_OR_UNLOAD_READ  32
#define LOBOT_SERVO_LED_CTRL_WRITE       33
#define LOBOT_SERVO_LED_CTRL_READ        34
#define LOBOT_SERVO_LED_ERROR_WRITE      35
#define LOBOT_SERVO_LED_ERROR_READ       36
 
#define PIN_SERIAL2_RX 15  // PA05
#define PAD_SERIAL2_RX (SERCOM_RX_PAD_1)
#define PIN_SERIAL2_TX 14  // PA04
#define PAD_SERIAL2_TX (UART_TX_PAD_0)


namespace Driver {
	
	void setup();

	// Helper Functions
	/******************************************************************************/

	// ID of desired servo to move, and destination angle from -120 to 120 degrees
	void moveTo(int, float);

	// ID of desired servo to spin, and velocity from -100 to 100 percent
	void spinAt(int, float);

	// Revolutions per minute
	int positionRead(int);

	// voltqage reading
	int getVoltage(int);

	/******************************************************************************/


	// Drive Functions
	/******************************************************************************/

	byte LobotCheckSum(byte[]);

	void LobotSerialServoMove(HardwareSerial&, uint8_t, int16_t, uint16_t);

	void LobotSerialServoStopMove(HardwareSerial&, uint8_t);

	void LobotSerialServoSetID(HardwareSerial&, uint8_t, uint8_t);

	void LobotSerialServoSetMode(HardwareSerial&, uint8_t, uint8_t, int16_t);

	void LobotSerialServoLoad(HardwareSerial&, uint8_t);

	void LobotSerialServoUnload(HardwareSerial&, uint8_t);

	int LobotSerialServoReceiveHandle(HardwareSerial&, byte*);

	int LobotSerialServoReadPosition(HardwareSerial&, uint8_t);

	int LobotSerialServoReadVin(HardwareSerial&, uint8_t);
	/******************************************************************************/
}

#endif