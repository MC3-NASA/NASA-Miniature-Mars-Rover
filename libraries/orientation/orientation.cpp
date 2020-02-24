#include "orientation.h"

/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (100)

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address

void orientation::setup(){
  Serial.println("Orientation Sensor Raw Data Test"); Serial.println("");

  /* Initialise the sensor */
  if (!bno.begin())
  {
	  /* There was a problem detecting the BNO055 ... check your connections */
	  Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
	  while (1);
  }
	delay(1000);
	bno.setExtCrystalUse(true);
}

void orientation::loop() {
	quaternion = bno.getQuat();
	sensors_event_t accel, comp;
	bno.getEvent(&accel, Adafruit_BNO055::VECTOR_LINEARACCEL);
	//bno.getEvent(&comp, Adafruit_BNO055::VECTOR_MAGNETOMETER);

	computeCompass(&comp);
	ax = acceleration.x();
	ay = acceleration.y();
	az = acceleration.z();
	computeAcceleration(&accel);
	trapezoidalIntegration();
}

void orientation::calibrate() {
	sensors_event_t event;
	bno.getEvent(&event, Adafruit_BNO055::VECTOR_LINEARACCEL);
	double x, y, z;
	x = event.acceleration.x;
	y = event.acceleration.y;
	z = event.acceleration.z;
	
	IIRFilter(x, y, z);

	acceleration.x() = xf;
	acceleration.y() = yf;
	acceleration.z() = zf;
}

void orientation::callibrateCompass() {
	uint8_t system, gyro, accel, mag = 0;
	while (system != 3)
	{
		bno.getCalibration(&system, &gyro, &accel, &mag);
		Serial.print("CALIBRATION: Sys=");
		Serial.print(system, DEC);
		Serial.print(" Gyro=");
		Serial.print(gyro, DEC);
		Serial.print(" Accel=");
		Serial.print(accel, DEC);
		Serial.print(" Mag=");
		Serial.println(mag, DEC);
		delay(100);
	}
	Serial.println(""); Serial.println("Calibrated");
	delay(2000);
}

void orientation::computeCompass(sensors_event_t * event) {
	heading = fmod(360 + toDegrees(quaternion.toEuler().x()), 360);
	incline = -toDegrees(quaternion.toEuler().z());
}

void orientation::computeAngle() {
	double x, y, z;
	orientationG.x() = 0;
	orientationG.y() = 0;
	orientationG.z() = 0;
	sensors_event_t event;
	
	bno.getEvent(&event);
	
	x = event.orientation.x;
	y = event.orientation.y;
	z = event.orientation.z;
	
	orientationG.x() = x;
	orientationG.y() = y;
	orientationG.z() = z;
	
	#ifdef DEBUG
	Serial.println("");
	Serial.println("Angle: ");
	Serial.print(orientationG.x());
	Serial.print(" ");
	Serial.print(orientationG.y());
	Serial.print(" ");
	Serial.print(orientationG.z());
	Serial.println("");
	#endif
}

void orientation::computeAcceleration(sensors_event_t * event) {
	double x, y, z;
	acceleration.x() = 0;
	acceleration.y() = 0;
	acceleration.z() = 0;

	x = event->acceleration.x;
	y = event->acceleration.y;
	z = event->acceleration.z;
	IIRFilter(x, y, z);

	if( abs(x) >= abs(( averageCalibration.x()* filterStrength )))
		acceleration.x() = xf;


	if (abs(y) >= abs((averageCalibration.y()* (filterStrength*0.8))))
		acceleration.y() = yf;


	if (abs(z) >= abs((averageCalibration.z()* (filterStrength*0.2))))
		acceleration.z() = zf;

}

void orientation::computeVelocity() {
	//Use Euler integration to compute velocity.
	velocity.x() += acceleration.x();
	velocity.y() += acceleration.y();
	velocity.z() += acceleration.z();
	velocity = velocity*0.01;
}

void orientation::computePosition() {
	//Use Euler integration to compute position.
	position.x() += velocity.x();
	position.y() += velocity.y();
	position.z() += velocity.z();
}

//Trapezoidal double integration to compute position and velocity from accelerometer.
//Delta time is 0.1 since we take a sample every 100 miliseconds.
void orientation::trapezoidalIntegration() {

	float basevelocityx = abs(velocity.x());
	float basevelocityy = abs(velocity.y());
	float basevelocityz = abs(velocity.z());

	float baseaccelerationx = ax;
	float baseaccelerationy = ay;
	float baseaccelerationz = az;

	float accelerationx = abs(acceleration.x());
	float accelerationy = abs(acceleration.y());
	float accelerationz = abs(acceleration.z());

	
	
	velocity.x() = basevelocityx + baseaccelerationx + ((accelerationx - baseaccelerationx) / 2);
	velocity.y() = basevelocityy + baseaccelerationy + ((accelerationy - baseaccelerationy) / 2);
	velocity.z() = basevelocityz + baseaccelerationz + ((accelerationz - baseaccelerationz) / 2);

	velocity = velocity* 0.1;//Delta time.


	
	if (abs(acceleration.x()) < tolerance) {
		velocity.x() = 0;
		basevelocityx = 0;
	}
	if (abs(acceleration.y()) < tolerance) {
		velocity.y() = 0;
		basevelocityy = 0;
	}
	if (abs(acceleration.z()) < tolerance) {
		velocity.z() = 0;
		basevelocityz = 0;
	}

	float forceX = basevelocityx + ((abs(velocity.x()) - basevelocityx) / 2);
	float forceY = basevelocityy + ((abs(velocity.y()) - basevelocityy) / 2);

	//position.x() = position.x() + basevelocityx + ((abs(velocity.x()) - basevelocityx) / 2);
	//position.y() = position.y() + basevelocityy + ((abs(velocity.y()) - basevelocityy) / 2);

	position.z() = position.z() + basevelocityz + ((abs(velocity.z()) - basevelocityz) / 2);
	applyForwardsForce(forceX+forceY);
}


//Applys linear acceleration according to the orientation of the rover using the Hamilton Product.
void orientation::applyForwardsForce(double f){
	imu::Vector<3> forwards = imu::Vector<3>(0, 1, 0); //The forwards vector, representing the Y plane going forward.
	forwardsVector = quaternion.rotateVector(forwards);//Performs the hamilton product using cross products.

	position = position+(forwardsVector*f);
}

//Infinite Impulse Response
void orientation::IIRFilter(double x, double y, double z) {
	double k = 0.9;
	xf = k * xf + (1.0 - k) * x;
	yf = k * yf + (1.0 - k) * y;
	zf = k * zf + (1.0 - k) * z;
}

String orientation::serialize() {

	double x = quaternion.x();
	double y = quaternion.y();
	double z = quaternion.z();
	double w = quaternion.w();
	String value = (String)"roverQ" + String(x, 6) + "," + String(y, 6) + "," + String(z, 6) + "," + String(w, 6);
	Serial.println(value);
	 x = position.x();
	 y = position.y();
	 z = position.z();
	value = (String)"roverP" + String(x, 6) + "," + String(y, 6) + "," + String(z, 6);
	Serial.println(value);



	//Additional Serializations

	/*
	x = forwardsVector.x();
	y = forwardsVector.y();
	z = forwardsVector.z();
	value = (String)"roverF" + String(x, 6) + "," + String(y, 6) + "," + String(z, 6);
	Serial.println(value);
	x = velocity.x();
	y = velocity.y();
	z = velocity.z();
	value = (String)"roverV" + String(x, 6) + "," + String(y, 6) + "," + String(z, 6);
	Serial.println(value);
	
	x = acceleration.x();
	y = acceleration.y();
	z = acceleration.z();
	value = (String)"roverA" + String(x, 6) + "," + String(y, 6) + "," + String(z, 6);
	Serial.println(value);

	x = averageCalibration.x();
	y = averageCalibration.y();
	z = averageCalibration.z();
	//Calibration Values
	value = (String)"roverC" + String(x, 6) + "," + String(y, 6) + "," + String(z, 6);
	Serial.println(value);

	*/
	Serial.print("HEADING: ");
	Serial.println(heading);
	return value;
}
