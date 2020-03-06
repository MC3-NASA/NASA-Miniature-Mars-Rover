#include "kalmanFilter.h"

void kalmanFilter::setup() {
	roverGPS.setup();
	orient.setup();
	calibrate();
	/*
	Q.vector_to_row(imu::Vector<2>(averageCalibration.y(), 0),0);
	Q.vector_to_row(imu::Vector<2>(0, averageCalibration.y()),1);

	R.vector_to_row(imu::Vector<2>(3, 0),0);
	R.vector_to_row(imu::Vector<2>(0, 3,1);


	Hx.vector_to_row(imu::Vector<2>(1, 0), 0);
	Hx.vector_to_row(imu::Vector<2>(0, 1), 1);

	Px.vector_to_row(imu::Vector<2>(1, 0), 0);
	Px.vector_to_row(imu::Vector<2>(0, 1), 1);


	Ux = orient.acceleration.x();
	
	Hy.vector_to_row(imu::Vector<2>(1, 0), 0);
	Hy.vector_to_row(imu::Vector<2>(0, 1), 1);

	Py.vector_to_row(imu::Vector<2>(1, 0), 0);
	Py.vector_to_row(imu::Vector<2>(0, 1), 1);


	Uy = orient.acceleration.y();
	*/
}

void kalmanFilter::loop() {
	orient.loop();
	roverGPS.loop();
	roverGPS.updateDistanceBearing(roverGPS.position, roverGPS.destination);
}

//Prediction step of the kalman filter.
void kalmanFilter::predict(){
/*
    predicted_state_estimate = self.A * self.current_state_estimate + self.B * control_vector
    predicted_prob_estimate = (self.A * self.current_prob_estimate) * numpy.transpose(self.A) + self.Q
*/
	/*
	//Predict.
	B.vector_to_row(imu::Vector<2>(1, 0.1, 0);
	B.vector_to_row(imu::Vector<2>(0, 1, 0);
	A.vector_to_row(imu::Vector<2>(1, 0.1, 0);
	A.vector_to_row(imu::Vector<2>(0, 1, 0);

	K = A*K + (B*U);
	P = A*P * (A.transpose()+Q);

	//Update

	Zlat.x() = roverGPS.position.x();
	Zlat.y() = roverGPS.speed;
	Zlon.x() = roverGPS.position.y();
	Zlon.y() = roverGPS.speed;
	*/

}

void kalmanFilter::calibrate() {
	int i = 0;
	for (; i < 250; i++) {
		orient.calibrate();

		averageCalibration.x() += abs(orient.acceleration.x());
		averageCalibration.y() += abs(orient.acceleration.y());
		averageCalibration.z() += abs(orient.acceleration.z());
		delay(20);
	}
	averageCalibration = averageCalibration*(1.0 / i);
	orient.xf = 0;
	orient.yf = 0;
	orient.zf = 0;

	orient.averageCalibration.x() = averageCalibration.x();
	orient.averageCalibration.y() = averageCalibration.y();
	orient.averageCalibration.z() = averageCalibration.z();

	//Gets the standard deviation.
	float x = 0;
	float y = 0;
	float z = 0;
	for (int j = 0; j < 250; j++) {
		orient.calibrate();
		x += pow(orient.acceleration.x() - averageCalibration.x(), 2);
		y += pow(orient.acceleration.y() - averageCalibration.y(), 2);
		z += pow(orient.acceleration.z() - averageCalibration.z(), 2);
		delay(10);
	}

	x *= (1.0 / i);
	y *= (1.0 / i);
	z *= (1.0 / i);
	averageCalibration.x() = sqrt(x);
	averageCalibration.y() = sqrt(y);
	averageCalibration.z() = sqrt(z);


}

bool kalmanFilter::destinationReached(){
	//Multi passes before the real condition check.
	//Checks if signal is given.
	if(abs(roverGPS.destination.x()) < 0.5f || abs(roverGPS.destination.y()) < 0.5f){
		//return roverGPS.traverseDestination();
	}
	if(abs(roverGPS.position.x()) < 0.5f || abs(roverGPS.position.y()) < 0.5f){
		return false;
	}
	if(roverGPS.distance <= destinationRadius){
		return roverGPS.traverseDestination();
	}
	return false;
}

void kalmanFilter::debug() {
	//Prints the GPS data. Absolute position.
	/*
	Serial.println(" GPS DATA ");
	Serial.println(roverGPS.position.x());
	Serial.println(roverGPS.position.y());
	Serial.println(" DESTINATION ");
	Serial.println(roverGPS.destination.x());
	Serial.println(roverGPS.destination.y());
	Serial.println(" DISTANCE ");
	Serial.println(String(roverGPS.distance, 8));
	Serial.println(" Bearing, Heading, Incline ");
	*/
	Serial.println(orient.heading);
	//Serial.println(orient.incline);
	Serial.println(roverGPS.bearing);
	//Serial.println("GPS SPEED: ");
	//Serial.println(roverGPS.speed);
	//Prints the accelerometer data.
	Serial.println(" POSITION DATA ");
	Serial.println(orient.position.x());
	Serial.println(orient.position.y());
	Serial.println(orient.position.z());
}

void kalmanFilter::serialize(){
	orient.serialize();
	roverGPS.serialize();
}