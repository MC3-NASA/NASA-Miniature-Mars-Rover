#pragma once
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <imumaths.h>
#include <Arduino.h>
#include <math.h>
// Possible vector values can be:
// - VECTOR_ACCELEROMETER - m/s^2
// - VECTOR_MAGNETOMETER  - uT
// - VECTOR_GYROSCOPE     - rad/s
// - VECTOR_EULER         - degrees
// - VECTOR_LINEARACCEL   - m/s^2
// - VECTOR_GRAVITY       - m/s^2
class orientation {
public:
	Adafruit_BNO055 bno = Adafruit_BNO055(-1, 0x28);
	imu::Quaternion quaternion; //Gets quaternion orientation of sensor.
	imu::Vector<3> position = imu::Vector<3>(0, 0, 0);
	imu::Vector<3> forwardsVector = imu::Vector<3>(0, 0, 0); //The homogenous point vector representing position rotated.
	imu::Vector<3> compass = imu::Vector<3>(0, 0, 0);
	imu::Vector<3> velocity = imu::Vector<3>(0, 0, 0);
	imu::Vector<3> acceleration = imu::Vector<3>(0, 0, 0);
	imu::Vector<3> averageCalibration = imu::Vector<3>(0, 0, 0);
	imu::Vector<3> orientationG = imu::Vector<3>(0, 0, 0);

	double heading = 0.0f;//Gets the heading for the rover.
	double incline = 0.0f;//Gets the incline of the rover from 180 to -180.
	double filterStrength = 6.25; //Filter calibration Strength.
	double xf, yf, zf;
	float ax, ay, az;
	float tolerance = 0.03; //Tolerance value for accelerometer sensitivity.

	float wheelRadius = 0.0593; //Approximate value of the wheels radius.
	float estimatedSpeed = 0.3; //Rough estimation of the max possible speed given the rate at which the wheels are turning.
	orientation() {};
	void setup();
	void loop();
	void calibrate();
	void computeAcceleration(sensors_event_t * event);
	void computeVelocity();
	void computePosition();
	float computeMagnitude();
	void IIRFilter(double x, double y, double z);
	void trapezoidalIntegration();
	void callibrateCompass();
	void computeCompass(sensors_event_t *event);
	void applyForwardsForce(double f);
	void computeAngle();
	float distanceTraveled = 0.0f; //Meters
	String serialize();
};
