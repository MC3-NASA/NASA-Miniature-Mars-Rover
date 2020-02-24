#include "DriveTrain.h"

//#define DEBUG


Wheel::Wheel(byte id, State type) {
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
	turn(0);
	
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
}

void DriveTrain::backward(float speed) {
	turn(0);
	
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

void DriveTrain::turn(float angle) {
	for(int i = 6; i < 10; i++) {
		
		#ifdef DEBUG
			Serial.println("Turn");
			Serial.print("Motor Wheel id: ");
			Serial.println(wheels[i]._id);
			Serial.println("");
		#endif
		
		if(wheels[i]._msState == SERVOL) {
			wheels[i].turn(angle);
		}else if(wheels[i]._msState == SERVOR) {
			wheels[i].turn(angle);
		}
	}
}


void DriveTrain::spin(float speed) {
	stop();
	
	for(int i = 6; i < 10; i++) {
		
		#ifdef DEBUG
			Serial.println("Turn");
			Serial.print("Motor Wheel id: ");
			Serial.println(wheels[i]._id);
			Serial.println("");
		#endif
		
		if(wheels[i]._msState == SERVOL) {
			if(wheels[i]._id == layout.leftBottomS)	wheels[i].turn(115);
			else									wheels[i].turn(70);
		}else if(wheels[i]._msState == SERVOR) {
			if(wheels[i]._id == layout.rightTopS)	wheels[i].turn(120);
			else									wheels[i].turn(70);
		}
	}
	
	float differential = (speed > 0) ? 10 : -10;
	
	for(int i = 0; i < 6; i++) {
		#ifdef DEBUG
			Serial.println("Forward Turning");
			Serial.print("Motor Wheel id: ");
			Serial.println(wheels[i]._id);
			Serial.println("");
		#endif
		
		if(wheels[i]._msState == MOTORL) {
			if(wheels[i]._id == layout.leftBack)	wheels[i].forward(-speed + differential);
			else									wheels[i].forward(speed + differential);
		}else if(wheels[i]._msState == MOTORR) {
			if(wheels[i]._id == layout.rightTop)	wheels[i].forward(-speed + -differential);
			else									wheels[i].forward(speed + -differential);
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