#pragma once
#include "sphereNode.h";
class internalMapping {
public:
	int nodesAmount = 10;
	float r = 1;
	sphereNode nodes[10];
	String map;
	void createMap();
	String createMap(vector3D position);
	void object(vector3D point);
	void unbound();
	int locator(vector3D point);
	String serialize();
};