#pragma once
#include <imumaths.h>
#include <Arduino.h>
#include <Adafruit_GPS.h>


#define GPSSerial Serial1
#define GPSECHO false

class Destination{
	public:
		imu::Vector<2> destination;
		Destination* next;
};
class RoverGPS {
public:
	Adafruit_GPS gps;
	Destination* destinations; //The current destination.
	Destination* headDestination;
	imu::Vector<2> destination = imu::Vector<2>(0, 0);
	imu::Vector<2> position = imu::Vector<2>(0, 0);
	double distance; //Distance in meters between two points.
	uint32_t timer = millis();
	double speed;
	double EarthRadius = 6373; //Mean earth radius Kilometers.
	double correction = 95.87;
	RoverGPS(){};
	void setup();
	void loop();
	void serialize();
	double calculateDistance(double latStart, double lonStart, double latDest, double lonDest); //Uses haversine formula to calculate distance.
	double calculateBearing(double latStart, double lonStart, double latDest, double lonDest); //Calculates Forwards azimuth
	void updateDistanceBearing(imu::Vector<2> pos, imu::Vector<2> dest);
	double bearing;
	bool traverseDestination();
	void setDestinations(double x, double y, int index);

};


