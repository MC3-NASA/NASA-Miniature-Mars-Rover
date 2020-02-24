#include "Distance.h"

//#define DEBUG

void Distance::setup(Save *data) {
	angleGoal = 0;
	currentAngle = 0;
	
	data->setup();
	
	kalman.setup();
	DriveTrain::setup();
	calibrate(data);
}

void Distance::loop() {
	kalman.loop();
}

void Distance::move(float distance) {
	
}

void Distance::spinA(float desiredAngle) {
	kalman.orient.computeAngle();
	
	if(desiredAngle > 0) {
	
		if(!goalAttemptingSpin) {
			spining = true;
			goalAttemptingSpin = true;
			
			#ifdef DEBUG
			Serial.println("");
			Serial.println("Reached spining");
			Serial.println("");
			#endif
			
			// set the goal
			if((desiredAngle + abs(kalman.orient.orientationG.x())) > 360) {
				angleGoal = (desiredAngle + abs(kalman.orient.orientationG.x())) - 360;
			}else {
				angleGoal = abs(kalman.orient.orientationG.x()) + desiredAngle;
			}
		}
		
		#ifdef DEBUG
		Serial.println("");
		Serial.print("Angle goal: ");
		Serial.println(angleGoal);
		Serial.println("");
		#endif
		
		if(spining) {
			currentAngle = abs(kalman.orient.orientationG.x());
			DriveTrain::spin(speed);
			
			Serial.println("angle goal");
			Serial.println(angleGoal);
			
			if(abs(currentAngle - angleGoal) < tolerance) {
				goalReachedSpin = true;
				spining = false;
				goalAttemptingSpin = false;
				DriveTrain::stop();
				Serial.println("Goal achieved");
				delay(5000);
			}
		}
	}else {
		
		if(!goalAttemptingSpin) {
			spining = true;
			goalAttemptingSpin = true;
			
			#ifdef DEBUG
			Serial.println("");
			Serial.println("Reached spining");
			Serial.println("");
			#endif
			
			// set the goal
			if((desiredAngle + abs(kalman.orient.orientationG.x())) < 0) {
				angleGoal = (desiredAngle + abs(kalman.orient.orientationG.x())) + 360;
			}else {
				angleGoal = abs(kalman.orient.orientationG.x()) + desiredAngle;
			}
		}
		
		#ifdef DEBUG
		Serial.println("");
		Serial.print("Angle goal: ");
		Serial.println(angleGoal);
		Serial.println("");
		#endif
		
		if(spining) {
			currentAngle = abs(kalman.orient.orientationG.x());
			DriveTrain::spin(-speed);
			
			Serial.println("angle goal");
			Serial.println(angleGoal);
			
			if(abs(currentAngle - angleGoal) < tolerance) {
				goalReachedSpin = true;
				spining = false;
				goalAttemptingSpin = false;
				DriveTrain::stop();
				Serial.println("Goal achieved");
				delay(5000);
			}
		}
	}
}

/*private functions*/
void Distance::calibrate(Save* data) {
	uint8_t system = 0, gyro = 0, accel = 0, mag = 0;
	
	#ifdef DEBUG
	Serial.println("Calibrating");
	Serial.print("System variable: ");
	Serial.println(system);
	#endif
	
	DriveTrain::forward(speed);				// Used to indicated when its calibrating for now
	
	while (system != 3) {
		kalman.orient.bno.getCalibration(&system, &gyro, &accel, &mag);
		
		#ifdef DEBUG
		Serial.print("CALIBRATION: Sys=");
		Serial.print(system, DEC);
		Serial.print(" Gyro=");
		Serial.print(gyro, DEC);
		Serial.print(" Accel=");
		Serial.print(accel, DEC);
		Serial.print(" Mag=");
		Serial.println(mag, DEC);
		#endif
		
		delay(100);
	}
	
	#ifdef DEBUG
	Serial.println("Data Saved");
	#endif
	
	data->saveCalibration(&kalman.orient.bno);
	
	DriveTrain::stop();
	#ifdef DEBUG
	Serial.println("Finished Calibrating");
	#endif
	delay(2000);
}