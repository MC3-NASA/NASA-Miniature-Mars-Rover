#pragma once
#include <Arduino.h>
#include <imumaths.h>
#include <math.h>
#include <sphereNode.h>
#include <Vector3D.h>
#include <save.h>

class Map
{
	// var
private:
	float radius;
	float circleradius;
	float distance;
	int n;
	sphereNode map[5];
	save mapLoader;
public:
	char GlobalMap[64][64]; //Global map given by map file on SD card.
	float latitude = 0; //The central latitude, given by Google Maps.
	float longitude = 0; // The central longitude given by Google Maps.
	int zoom = 18; //The zoom level given by Google Maps. By default 18.
	Map(int radius, float sphereradius);
	Map();
	void flush();
	void update(float distance, float angle);
	bool read(int index);
	bool read(vector3D v);
	String serialize();
	String Sizeserialize();
	void loadMap();
	void findPath();
	void printMap();
	imu::Vector<2> getRealPosition(float x, float y, imu::Vector<2> vec); //Converts an index of the map to a latitude and longitude vector.

	void setup();
	void loop();
	//void WayPointHandler();
	void loadWayPoints();
};