#pragma once
//Created by Noah Williams on 9/9/2019.
//Represents a single sphere and all of its functions.
#include "Vector3D.h"
#include "math.h"
using namespace std;
class sphereNode {
public:
	vector3D position;
	int index; //index where it exists in navigation mesh.
	bool traversable = true; //Is there an object here?
	sphereNode() {
	};
	sphereNode(vector3D locate) {
		position = locate;
	};

	float distance(vector3D d) {
		vector3D pos = position;

		return (pos-d).mag();
	};
	//Checks if rover is bounded inside sphere.
	bool bounded(vector3D a, float r) {

		float d = distance(a);
		if (r >= d) {
			return true;
		}
		return false;
	}
};
