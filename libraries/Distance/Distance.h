#ifndef _Distance_h
#define _Distance_h

#include "DriveTrain.h"
#include "kalmanFilter.h"
#include "Save.h"

class Distance : public DriveTrain {
	public:
	float speed = 50;
	bool spining=false;
	bool goalReachedSpin=false, goalAttemptingSpin=false;
	kalmanFilter kalman;
	float currentAngle;
	float angleGoal;
	float tolerance = 10; // degrees
	
	public:
	void setup(Save*);
	void loop();
	void move(float);			// forward or backward distance
	void spinA(float);			// spin angle (degrees) (-) or (+)
	
								// Stop is built into the implementation
								//  once goal is achieved it will stop
	
	private:
	void calibrate(Save*);			// Setup the IMU device, implementation will be hidden since setup function will call it
};

#endif