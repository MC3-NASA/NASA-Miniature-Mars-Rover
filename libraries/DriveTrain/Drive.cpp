#include "Drive.h"

//#define Serial1 Serial2

/* Helper Functions */
void Driver::setup() {
	Serial2.begin(115200);
}

void Driver::moveTo(int servoId, float destination) {
	// Input is in angle relative to center as zero, from -120 to 120.
	// Negative numbers clockwise (when looking at face of servo.)

	// Output to LewanSoul API is from 0 to 1000, with 500 as center.
	// Interpreted as movement from -120 to 120 degrees.
	LobotSerialServoMove(Serial2, servoId, (destination * (500.0/120.0))+500, 200);
}

void Driver::spinAt(int servoId, float velocity) {
	// Input should be from -100 to 100, so multiply by 10 to get
	// the -1000 to 1000 range desired by LewanSoul API
	LobotSerialServoSetMode(Serial2, servoId, 1, velocity * 10);
}

int Driver::positionRead(int id) {
	return LobotSerialServoReadPosition(Serial2, id);
}

int Driver::getVoltage(int id) {
	return LobotSerialServoReadVin(Serial2, id);
}


/* Private Functions */
byte Driver::LobotCheckSum(byte buf[]) {
	byte i;
	uint16_t temp = 0;
	for (i = 2; i < buf[3] + 2; i++) {
	temp += buf[i];
	}
	temp = ~temp;
	i = (byte)temp;
	return i;
}

void Driver::LobotSerialServoMove(HardwareSerial &SerialX, uint8_t id, int16_t position, uint16_t time) {
	byte buf[10];
	if(position < 0)
	position = 0;
	if(position > 1000)
	position = 1000;
	buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
	buf[2] = id;
	buf[3] = 7;
	buf[4] = LOBOT_SERVO_MOVE_TIME_WRITE;
	buf[5] = GET_LOW_BYTE(position);
	buf[6] = GET_HIGH_BYTE(position);
	buf[7] = GET_LOW_BYTE(time);
	buf[8] = GET_HIGH_BYTE(time);
	buf[9] = LobotCheckSum(buf);
	SerialX.write(buf, 10);
}

void Driver::LobotSerialServoStopMove(HardwareSerial &SerialX, uint8_t id) {
	byte buf[6];
	buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
	buf[2] = id;
	buf[3] = 3;
	buf[4] = LOBOT_SERVO_MOVE_STOP;
	buf[5] = LobotCheckSum(buf);
	SerialX.write(buf, 6);
}

void Driver::LobotSerialServoSetID(HardwareSerial &SerialX, uint8_t oldID, uint8_t newID) {
	byte buf[7];
	buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
	buf[2] = oldID;
	buf[3] = 4;
	buf[4] = LOBOT_SERVO_ID_WRITE;
	buf[5] = newID;
	buf[6] = LobotCheckSum(buf);
	SerialX.write(buf, 7);
}

void Driver::LobotSerialServoSetMode(HardwareSerial &SerialX, uint8_t id, uint8_t Mode, int16_t Speed) {
	byte buf[10];

	buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
	buf[2] = id;
	buf[3] = 7;
	buf[4] = LOBOT_SERVO_OR_MOTOR_MODE_WRITE;
	buf[5] = Mode;
	buf[6] = 0;
	buf[7] = GET_LOW_BYTE((uint16_t)Speed);
	buf[8] = GET_HIGH_BYTE((uint16_t)Speed);
	buf[9] = LobotCheckSum(buf);

	SerialX.write(buf, 10);
}
void Driver::LobotSerialServoLoad(HardwareSerial &SerialX, uint8_t id) {
	byte buf[7];
	buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
	buf[2] = id;
	buf[3] = 4;
	buf[4] = LOBOT_SERVO_LOAD_OR_UNLOAD_WRITE;
	buf[5] = 1;
	buf[6] = LobotCheckSum(buf);

	SerialX.write(buf, 7);
}

void Driver::LobotSerialServoUnload(HardwareSerial &SerialX, uint8_t id) {
	byte buf[7];
	buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
	buf[2] = id;
	buf[3] = 4;
	buf[4] = LOBOT_SERVO_LOAD_OR_UNLOAD_WRITE;
	buf[5] = 0;
	buf[6] = LobotCheckSum(buf);

	SerialX.write(buf, 7);
}

int Driver::LobotSerialServoReceiveHandle(HardwareSerial &SerialX, byte *ret) {
	bool frameStarted = false;
	bool receiveFinished = false;
	byte frameCount = 0;
	byte dataCount = 0;
	byte dataLength = 2;
	byte rxBuf;
	byte recvBuf[32];
	byte i;

	while (SerialX.available()) {
		rxBuf = SerialX.read();
		delayMicroseconds(100);
		if (!frameStarted) {
			if (rxBuf == LOBOT_SERVO_FRAME_HEADER) {
				frameCount++;
				if (frameCount == 2) {
					frameCount = 0;
					frameStarted = true;
					dataCount = 1;
				}
			}
			else {
				frameStarted = false;
				dataCount = 0;
				frameCount = 0;
			}
		}

		if (frameStarted) {
			recvBuf[dataCount] = (uint8_t)rxBuf;
			if (dataCount == 3) {
				dataLength = recvBuf[dataCount];

				if (dataLength < 3 || dataCount > 7) {
					dataLength = 2;
					frameStarted = false;
				}
			}
			dataCount++;
			if (dataCount == dataLength + 3) {
				
				if (LobotCheckSum(recvBuf) == recvBuf[dataCount - 1]) {
					frameStarted = false;
					memcpy(ret, recvBuf + 4, dataLength);
					return 1;
				}

				return -1;
			}
		}
	}
}

int Driver::LobotSerialServoReadPosition(HardwareSerial &SerialX, uint8_t id) {
	int count = 10000;
	int ret;
	byte buf[6];

	buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
	buf[2] = id;
	buf[3] = 3;
	buf[4] = LOBOT_SERVO_POS_READ;
	buf[5] = LobotCheckSum(buf);

	while (SerialX.available())
		SerialX.read();

	SerialX.write(buf, 6);

	while (!SerialX.available()) {
		count -= 1;
		if (count < 0)
		  return -1;
	}

	if (LobotSerialServoReceiveHandle(SerialX, buf) > 0)
		ret = BYTE_TO_HW(buf[2], buf[1]);
	else
		ret = -1;

	return ret;
}

int Driver::LobotSerialServoReadVin(HardwareSerial &SerialX, uint8_t id) {
	int count = 10000;
	int ret;
	byte buf[6];

	buf[0] = buf[1] = LOBOT_SERVO_FRAME_HEADER;
	buf[2] = id;
	buf[3] = 3;
	buf[4] = LOBOT_SERVO_VIN_READ;
	buf[5] = LobotCheckSum(buf);


	while (SerialX.available())
		SerialX.read();

	SerialX.write(buf, 6);

	while (!SerialX.available()) {
		count -= 1;
		if (count < 0)
		  return -2048;
	}

	if (LobotSerialServoReceiveHandle(SerialX, buf) > 0)
		ret = (int16_t)BYTE_TO_HW(buf[2], buf[1]);
	

	return ret;
}