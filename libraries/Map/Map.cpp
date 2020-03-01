#include "Map.h"
Map::Map(int nodes, float sphereradius)
{
	n = nodes;
	radius = sphereradius;

	circleradius = n * radius;
	distance = circleradius / (2 * PI);
	float angle = (2 * PI) / n;
	for (int i = 0; i < n; i++)
	{
		map[i].position.x = distance * cos(angle * i);
		map[i].position.y = distance * sin(angle * i);
		map[i].position.z = 0;
	}
}

Map::Map(){

}

void Map::setup(){
	mapLoader.setup();
}

void Map::loop(){
}

void Map::flush()
{
	for (int i = 0; i < sizeof(map)/sizeof(*map); i++)
	{
		map[i].traversable = true;
	}
}

void Map::update(float distance, float angle)
//Distance should be in meters, angle should be in degrees
{
	angle = angle * (PI / 180);
	float xcoord = distance * cos(angle);
	float ycoord = distance * sin(angle);
	vector3D v(xcoord, ycoord, 0);
	for (int i = 0; i < n; i++)
	{
		if (map[i].distance(v) <= radius)
		{
			map[i].traversable = false;
		}
	}
}

bool Map::read(int index)
{
	return (map[index].traversable);
}
bool Map::read(vector3D v)
{
	for (int i = 0; i < n; i++)
	{
		if (map[i].distance(v) <= radius)
		{
			return (map[i].traversable);
		}
	}
}

String Map::serialize() {
	String mapData = "";
	//nodes = new sphereNode[nodesAmount];
	//delete[] nodes;
	for (int i = 0; i < n; i++) {
		mapData += map[i].position.serialize();
	}
	return (String)"m_p"+ mapData;
}

String Map::Sizeserialize() {
	return (String)"n_n" + n;
}

void Map::loadMap(){
	mapLoader.loadMap("MAPROVER.txt", GlobalMap, &latitude, &longitude, &zoom);
}


void Map::printMap(){
	for(int i = 0; i < 64; i++){
		for(int j = 0; j < 64; j++){
		Serial.print(GlobalMap[i][j]);
		}
	}
	Serial.println("");
}

//Performs Dijstra's Algorithm on a binary 64X64 matrix. By using BFS.
//The path is then stored as a linked list of destinations (Vectors) for the Rover to travel to.
void findPath(){

}

//Converts an index of the GlobaMap to a real world location, return latitude, and longitude.
imu::Vector<2> Map::getRealPosition(int x, int y, imu::Vector<2> vec){
	x = floor(x/10)*10;
	y = floor(y/10)*10;
	double multiplier = cos(latitude * M_PI / 180);
    double degreesPerPixelX = 360 / pow(2, zoom + 8);
    double degreesPerPixelY = 360 / pow(2, zoom + 8) *multiplier;
    double latFinal = latitude - degreesPerPixelY * (y - 640 / 2);
    double lonFinal = longitude + degreesPerPixelX * (x - 640 / 2);
	vec.x() = latFinal;
	vec.y() = lonFinal;
    return vec;
}

//Loads just a list of points instead of entire map. Can load up to 12 points.
void Map::loadWayPoints(){
	mapLoader.loadMap("WAYPOINTS.txt", GlobalMap, &latitude, &longitude, &zoom);
}
