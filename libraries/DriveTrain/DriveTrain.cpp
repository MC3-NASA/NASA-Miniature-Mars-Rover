#include "DriveTrain.h"

//#define DEBUG


Wheel::Wheel(byte id, MotorState type) {
	_id = id;
	_msState = type;
}

void Wheel::forward(float speed) {
	Driver::spinAt(_id, speed);
}

void Wheel::backward(float speed) {
	Driver::spinAt(_id, -speed);
}

void Wheel::turn(float angle) {
	Driver::moveTo(_id, angle);
}

void Wheel::stop() {
	Driver::spinAt(_id, 0);
}


void DriveTrain::setup() {
	layout.leftTop = 25;
  	layout.leftMid = 21;
  	layout.leftBack = 20;
  	layout.rightTop = 27;
  	layout.rightMid = 22;
  	layout.rightBack = 28;
  	layout.leftTopS = 23;
  	layout.leftBottomS = 24;
  	layout.rightTopS = 29;
  	layout.rightBottomS = 26;
	
	
	 for(int i = 0; i < 3; i++) {
		wheels[i] = Wheel(layout.motorsLeft[i], MOTORL);
	 }
	 
	 for(int i = 3; i < 6; i++) {
		wheels[i] = Wheel(layout.motorsRight[i-3], MOTORR);
	 }
	 
	 for(int i = 6; i < 8; i++) {
		wheels[i] = Wheel(layout.servosLeft[i-6], SERVOL);
	 }
	 
	 for(int i = 8; i < 10; i++) {
		 wheels[i] = Wheel(layout.servosRight[i-8], SERVOR);
	 }
	 
	 Driver::setup();
}

void DriveTrain::forward(float speed) {
	MainSpeed = speed;
	//Used for first initial start.
	if(abs(V1)+abs(V2)+abs(V3)+abs(V4)+abs(V5)+abs(V6) < 0.1f){
		CalculateWheelSpeed(speed, 0);
	}
	if(abs(speed) < 1){
		V1 = 0;
		V2 = 0;
		V3 = 0;
		V4 = 0;
		V5 = 0;
		V6 = 0;
	}
	for(int i = 0; i < 6; i++){
		if(wheels[i]._id == layout.leftTop) wheels[i].forward(V1);
		if(wheels[i]._id == layout.leftMid) wheels[i].forward(V2);
		if(wheels[i]._id == layout.leftBack) wheels[i].forward(V3);

		if(wheels[i]._id == layout.rightTop) wheels[i].forward(-V4);
		if(wheels[i]._id == layout.rightMid) wheels[i].forward(-V5);
		if(wheels[i]._id == layout.rightBack) wheels[i].forward(-V6);
	}
	/*
	for(int i = 0; i < 6; i++) {
		
		#ifdef DEBUG
			Serial.println("Forward");
			Serial.print("Motor Wheel id: ");
			Serial.println(wheels[i]._id);
			Serial.println("");
		#endif
		
		if(wheels[i]._msState == MOTORL) {
			wheels[i].forward(speed);
		}else if(wheels[i]._msState == MOTORR) {
			wheels[i].forward(-speed);
		}
	}
	*/
}

void DriveTrain::backward(float speed) {
	MainSpeed = speed;
	for(int i = 0; i < 6; i++) {
		if(wheels[i]._msState == MOTORL) {
			
			#ifdef DEBUG
				Serial.println("Backward");
				Serial.print("Motor Wheel id: ");
				Serial.println(wheels[i]._id);
				Serial.println("");
			#endif
			
			wheels[i].backward(speed);
		}else if(wheels[i]._msState == MOTORR) {
			wheels[i].backward(-speed);
		}
	}
}


void DriveTrain::turn(float angle, byte motors) {
	CalculateWheelSpeed(MainSpeed, angle);
	for(int i = 6; i < 10; i++) {
		
		#ifdef DEBUG
			Serial.println("Turn");
			Serial.print("Motor Wheel id: ");
			Serial.println(wheels[i]._id);
			Serial.println("");
		#endif
		
		if(wheels[i]._msState == SERVOL) {
			if(wheels[i]._id == layout.leftTopS)		(((motors) & (1<<(0)))) ?  wheels[i].turn(angle) : wheels[i].turn(0);
			if(wheels[i]._id == layout.leftBottomS)		(((motors) & (1<<(1)))) ?  wheels[i].turn(-angle) : wheels[i].turn(0);
		}else if(wheels[i]._msState == SERVOR) {
			if(wheels[i]._id == layout.rightTopS)		(((motors) & (1<<(2)))) ?  wheels[i].turn(angle) : wheels[i].turn(0);
			if(wheels[i]._id == layout.rightBottomS)	(((motors) & (1<<(3)))) ?  wheels[i].turn(-angle) : wheels[i].turn(0);
		}
	}

	forward(MainSpeed);
}

void DriveTrain::turn(float angle, int speed, byte motors) {
	CalculateWheelSpeed(MainSpeed, angle);
  /*int negativeOne = 1;
  if(angle < 0)                               negativeOne = -1;
  if(angle > 0)                               negativeOne = 1;
  
  for(int i = 0; i < 10; i++) {
    if(wheels[i]._msState == SERVOL) {
		if(wheels[i]._id == layout.leftTopS)	    (((motors) & (1<<(0)))) ?  wheels[i].turn(angle) : wheels[i].turn(0);
		if(wheels[i]._id == layout.leftBottomS)		(((motors) & (1<<(1)))) ?  wheels[i].turn((angle * negativeOne)) : wheels[i].turn(0);
	}else if(wheels[i]._msState == SERVOR) {
		if(wheels[i]._id == layout.rightTopS)     	(((motors) & (1<<(2)))) ?  wheels[i].turn(angle) : wheels[i].turn(0);
		if(wheels[i]._id == layout.rightBottomS)	(((motors) & (1<<(3)))) ?  wheels[i].turn(-(angle * negativeOne)) : wheels[i].turn(0);
	}
 
    if(wheels[i]._msState == MOTORL) {
		if(wheels[i]._id == layout.leftTop)     	wheels[i].forward(speed);
		if(wheels[i]._id == layout.leftMid)     	(angle < 0) ? wheels[i].forward(-speed) : wheels[i].forward(speed);
		if(wheels[i]._id == layout.leftBack)		  wheels[i].forward((speed));
	}else if(wheels[i]._msState == MOTORR) {
		if(wheels[i]._id == layout.rightTop)		wheels[i].forward(-speed);
		if(wheels[i]._id == layout.rightMid)		(angle > 0) ? wheels[i].forward(speed) : wheels[i].forward(-speed);
		if(wheels[i]._id == layout.rightBack)		wheels[i].forward((speed));
	}
  }*/
  
	for(int i = 0; i < 6; i++) {
		
		#ifdef DEBUG
			Serial.println("Forward");
			Serial.print("Motor Wheel id: ");
			Serial.println(wheels[i]._id);
			Serial.println("");
		#endif
		
		if(wheels[i]._msState == MOTORL) {
			if(wheels[i]._id == layout.leftMid)     	(angle < 0) ? wheels[i].forward(-speed) : wheels[i].forward(speed);
			else	wheels[i].forward(speed);
		}else if(wheels[i]._msState == MOTORR) {
			if(wheels[i]._id == layout.rightMid)		(angle > 0) ? wheels[i].forward(speed) : wheels[i].forward(-speed);
			else	wheels[i].forward(-speed);
		}
	}

  for(int i = 6; i < 10; i++) {
		
		#ifdef DEBUG
			Serial.println("Turn");
			Serial.print("Motor Wheel id: ");
			Serial.println(wheels[i]._id);
			Serial.println("");
		#endif
		
		if(wheels[i]._msState == SERVOL) {
			if(wheels[i]._id == layout.leftTopS)		(((motors) & (1<<(0)))) ?  wheels[i].turn(angle) : wheels[i].turn(0);
			if(wheels[i]._id == layout.leftBottomS)		(((motors) & (1<<(1)))) ?  wheels[i].turn(-angle) : wheels[i].turn(0);
		}else if(wheels[i]._msState == SERVOR) {
			if(wheels[i]._id == layout.rightTopS)		(((motors) & (1<<(2)))) ?  wheels[i].turn(angle) : wheels[i].turn(0);
			if(wheels[i]._id == layout.rightBottomS)	(((motors) & (1<<(3)))) ?  wheels[i].turn(-angle) : wheels[i].turn(0);
		}
	}
	
	
}


void DriveTrain::spin(float speed) {
	MainSpeed = speed;
	CalculateWheelSpeed(MainSpeed, 90);
	for (int i = 6; i < 10; i++)
	{

#ifdef DEBUG
		Serial.println("Turn");
		Serial.print("Motor Wheel id: ");
		Serial.println(wheels[i]._id);
		Serial.println("");
#endif

		if (wheels[i]._msState == SERVOL)
		{
			if (wheels[i]._id == layout.leftBottomS)
				wheels[i].turn(115);
			else
				wheels[i].turn(70);
		}
		else if (wheels[i]._msState == SERVOR)
		{
			if (wheels[i]._id == layout.rightTopS)
				wheels[i].turn(120);
			else
				wheels[i].turn(70);
		}
	}

	float differential = (speed > 0) ? 10 : -10;

	for (int i = 0; i < 6; i++)
	{
#ifdef DEBUG
		Serial.println("Forward Turning");
		Serial.print("Motor Wheel id: ");
		Serial.println(wheels[i]._id);
		Serial.println("");
#endif

		if (wheels[i]._msState == MOTORL)
		{
			if (wheels[i]._id == layout.leftBack)
				wheels[i].forward(-speed + differential);
			else
				wheels[i].forward(speed + differential);
		}
		else if (wheels[i]._msState == MOTORR)
		{
			if (wheels[i]._id == layout.rightTop)
				wheels[i].forward(-speed + -differential);
			else
				wheels[i].forward(speed + -differential);
		}
	}
}

void DriveTrain::stop() {
	for(int i = 0; i < 10; i++) {
		if(wheels[i]._msState == MOTORL) {
			wheels[i].stop();
		}else if(wheels[i]._msState == MOTORR) {
			wheels[i].stop();
		}else if(wheels[i]._msState == SERVOL) {
			wheels[i].turn(0);
		}else if(wheels[i]._msState == SERVOR) {
			wheels[i].turn(0);
		}
	}
}

void DriveTrain::CalculateWheelSpeed(float speed, float direction){

	    if(abs(direction) > 0){
			float turningRadius = 250-abs(((float)direction / 100) * 250);
			turningRadius = constrain(turningRadius, 20, 250);
			V1 = abs(speed * (sqrt(d3 * d3 + pow((d1 + turningRadius), 2))) / (turningRadius + d4) );
			V2 = abs(speed);
			V3 = abs(speed * (sqrt(d2 * d2 + pow((d1 + turningRadius), 2))) / (turningRadius + d4));

			V4 = abs(speed * (sqrt(d3 * d3 + pow((turningRadius - d1), 2))) / (turningRadius + d4));
			V5 = abs(speed * (turningRadius-d4)/(turningRadius+d4));
			V6 = abs((speed * (sqrt(d2 * d2 + pow((turningRadius - d1), 2))) / (turningRadius + d4) ));

			V1 = constrain(V1, 0, 100);
			V2 = constrain(V2, 0, 100);
			V3 = constrain(V3, 0, 100);


			//Inside wheels when turning right by defualt.
			V4 = constrain(V4, 0, 100);
			V5 = constrain(V5, 0, 100);
			V6 = constrain(V6, 0, 100);
		}else{
			V1 = speed;
			V2 = speed;
			V3 = speed;
			V4 = speed;
			V5 = speed;
			V6 = speed;
		}
		

		//When it turns left the diagram shifts, inner wheels swap speeds.
		if(direction > 0){
			swapSpeeds(&V1, &V4);
			swapSpeeds(&V2, &V5);
			swapSpeeds(&V3, &V6);
		}

}

bool DriveTrain::zeroRadiusTurn(float desiredAngle, orientation orient){

	float speed = 90;
	float tolerance = 10; // degrees
	orient.computeAngle();
	
	if(desiredAngle > 0) {
	
		if(!goalAttemptingSpin) {
			spining = true;
			goalAttemptingSpin = true;
			// set the goal
			if((desiredAngle + abs(orient.orientationG.x())) > 360) {
				angleGoal = (desiredAngle + abs(orient.orientationG.x())) - 360;
			}else {
				angleGoal = abs(orient.orientationG.x()) + desiredAngle;
			}
		}
		
		if(spining) {
			currentAngle = abs(orient.orientationG.x());
			spin(speed);
			
			
			if(abs(currentAngle - angleGoal) < tolerance) {
				goalReachedSpin = true;
				spining = false;
				goalAttemptingSpin = false;
				stop();
			}
		}
	}else {
		
		if(!goalAttemptingSpin) {
			
			
			// set the goal
			if((desiredAngle + abs(orient.orientationG.x())) < 0) {
				angleGoal = (desiredAngle + abs(orient.orientationG.x())) + 360;
			}else {
				angleGoal = abs(orient.orientationG.x()) + desiredAngle;
			}
		}
		if(spining) {
			currentAngle = abs(orient.orientationG.x());
			spin(-speed);
			
			Serial.println("angle goal");
			Serial.println(angleGoal);
			
			if(abs(currentAngle - angleGoal) < tolerance) {
				goalReachedSpin = true;
				spining = false;
				goalAttemptingSpin = false;
				stop();
			}
		}
	}

	return goalReachedSpin;
}

//Swaps two numbers used when going left or right.
void DriveTrain::swapSpeeds(float *x, float *y){
	float t;
	t = *x;
	*x = *y;
	*y = t;
}
