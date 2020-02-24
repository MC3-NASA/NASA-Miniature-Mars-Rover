#ifndef _DriveTrain_h
#define _DriveTrain_h

#include "Drive.h"

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

typedef enum {MOTORL, MOTORR, SERVOL, SERVOR, NA} State;// States an individual servo can be in dependent on the orientation(LEFT, RIGHT)

class Wheel {
	public:
	Wheel(byte, State);
	Wheel() {_id = 0; _msState = NA; }
	void forward(float);					// forward speed
	void backward(float);					// backward speed
	void turn(float);						// turn angle
	void stop();							// stop a motor
	
	public:
	byte _id;
	State _msState;
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
	void turn(float); 						// turn a specified angle for all turning motors
};


#endif