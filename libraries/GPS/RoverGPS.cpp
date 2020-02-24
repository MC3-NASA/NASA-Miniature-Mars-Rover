#include "RoverGPS.h"
void RoverGPS::setup() {
	gps = Adafruit_GPS(&GPSSerial);
	gps.begin(9600);
	gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
	gps.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
	gps.sendCommand(PGCMD_ANTENNA);
	delay(1000);

}

void RoverGPS::loop() {

	gps.read();
	if (gps.newNMEAreceived()) {
		gps.lastNMEA();
		if (!gps.parse(gps.lastNMEA()))
			return;
	}
	if (timer > millis()) timer = millis();
	if (millis() - timer > 1000) {
		timer = millis(); // reset the timer
		if (gps.fix) {

			position.x() =  gps.latitudeDegrees;//North and South.
			position.y() = gps.longitudeDegrees;//East and West.
			destination.x() = 40.081892; 
			destination.y() = -75.163829;
			speed = gps.speed;
		}

		calculateBearing(position.x(), position.y(), destination.x(), destination.y()); //Recalculates every 1 seconds.
	}

	
}

double RoverGPS::calculateBearing(double latStart, double lonStart, double latDest, double lonDest) {
	latStart = toRadians(latStart);
	latDest = toRadians(latDest);
	lonStart = toRadians(lonStart);
	lonDest = toRadians(lonDest);
	double y = sin(lonDest - lonStart)*cos(latDest);
	double x = cos(latStart)*sin(latDest) - sin(latStart)*cos(latDest)*cos(lonDest-lonStart);
	double bear = toDegrees(atan2(y, x));
	bearing = fmodf((360 + bear), 360.00); //ensures that degree is between 0 and 360 just like heading.
	return bearing;
}

//Calculates using haversine formula.
double RoverGPS::calculateDistance(double latStart, double lonStart, double latDest, double lonDest) {


	latStart = toRadians(latStart);
	lonStart = toRadians(lonStart);

	latDest = toRadians(latDest);
	lonDest = toRadians(lonDest);

	double phiStart = latStart;
	double phiDest = latDest;

	double deltaLat = (latDest - latStart);
	double deltaLon = (lonDest - lonStart);


	double alpha = sin(deltaLat / 2)*sin(deltaLat / 2)+
				   
				   cos(phiStart)*cos(phiDest)*
				   
				   sin(deltaLon / 2)*sin(deltaLon / 2);

	double c = 2 * atan2(sqrt(alpha), sqrt(1-alpha));
	distance = (EarthRadius * c)*1000;
	return distance;
}

void RoverGPS::updateDistanceBearing(imu::Vector<2> pos, imu::Vector<2> dest){
	calculateBearing(pos.x(), pos.y(), dest.x(), dest.y());
	calculateDistance(pos.x(), pos.y(), dest.x(), dest.y());
}

void RoverGPS::serialize() {
	String value = (String)"Position: " + String(position.x(), 8) + "," + String(position.y(), 8);
	Serial.println(value);
	value = (String)"Destination: " + String(destination.x(), 8) + "," + String(destination.y(), 8);
	Serial.println(value);
	Serial.print("GPS_SPEED");
	Serial.println(speed);
	Serial.print("Bearing | Distance  ");
	Serial.print(bearing);
	Serial.print(" | ");
	Serial.println(String(distance, 8));
}