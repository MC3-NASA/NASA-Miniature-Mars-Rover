#ifndef _DriveTrain_h
#define _DriveTrain_h

#include "Drive.h"
#include <orientation.h>

struct DriveTrainLayout {
	union {
		struct {									// This part of data represents individual wheels
			byte leftTop, leftMid,	leftBack;
			byte rightTop, rightMid, rightBack;
			byte leftTopS, leftBottomS;
			byte rightTopS, rightBottomS;
		};
		struct {									// This part of data represents the the data above as a whole line by line
			byte motorsLeft[3];
			byte motorsRight[3];
			byte servosLeft[2];
			byte servosRight[2];
		};
	};
};

typedef enum {MOTORL, MOTORR, SERVOL, SERVOR, NA} MotorState;// States an individual servo can be in dependent on the orientation(LEFT, RIGHT)
typedef enum {LEFTTOP = (0x81), LEFTBOTTOM = (0x82), RIGHTTOP = (0x84), RIGHTBOTTOM = (0x88)} MotorsReg;

class Wheel {
	public:
	Wheel(byte, MotorState);
	Wheel() {_id = 0; _msState = NA; }
	void forward(float);					// forward speed
	void backward(float);					// backward speed
	void turn(float);						// turn angle
	void stop();							// stop a motor
	
	public:
	byte _id;
	MotorState _msState;
};

class DriveTrain {
	public:
	DriveTrain() {}
	DriveTrainLayout layout;;
	Wheel wheels[10];
	
	void setup();							// Setup the layout and the wheels
	void forward(float);					// Forward for all driving motors
	void backward(float);					// similar to forward but reverse(technically not nessarcy but separate implementation for naming purpose)
	void spin(float);						// Spin in place for speed
	void stop();							// Stops all motors and all servo are set to angle zero
	void turn(float, byte); 					// turn a specified angle for all turning motors
	void turn(float, int, byte);
	void CalculateWheelSpeed(float speed, float turningRadius);
	bool zeroRadiusTurn(float angle, orientation orient); //Turns to a specified angle using the gyroscope.

	float d1 = 7.254f; //inches. Horizontal distance between middle rover and corner wheels.
    float d2 = 10.5f; //Verticle distance middle rover and corner wheels.
    float d3 = 10.5f; //Verticle rover front wheels
    float d4 = 10.073f; //Horizontal distance center wheels.

	float V1 = 0; //Speed of front left wheel.
    float V2 = 0; //Speed of middle left  wheel.
    float V3 = 0; //Speed of bottom left wheel.

    float V4 = 0; //Speed of front right wheel.
    float V5 = 0; //Speed of middle right wheel.
    float V6 = 0; //Speed of bottom right wheel.
	float MainSpeed = 0;
	float minimumRadius = 20;

	bool goalReachedSpin=false, goalAttemptingSpin=false;
	float currentAngle;
	float angleGoal;
	bool spining=false;
	
};


#endif
