#pragma once
#include <math.h>
#include <Arduino.h>
using namespace std;
class vector3D {
public:
	float x;
	float y;
	float z;
	vector3D() {
		x = 0;
		y = 0;
		z = 0;
	};
	vector3D(float xp, float yp, float zp) {
		x = xp;
		y = yp;
		z = zp;
	};

	void set(vector3D s) {
		x = s.x;
		y = s.y;
		z = s.z;
	};

	vector3D operator * (float s) {
		x *= s;
		y *= s;
		z *= s;
		return vector3D(x, y, z);
	};

	vector3D operator + (float s) {
		x += s;
		y += s;
		z += s;
		return vector3D(x, y, z);
	};

	vector3D operator + (vector3D s) {
		x += s.x;
		y += s.y;
		z += s.z;
		return vector3D(x, y, z);
	};

	vector3D operator - (float s) {
		x -= s;
		y -= s;
		z -= s;
		return vector3D(x, y, z);
	};

	vector3D operator - (vector3D s) {
		x -= s.x;
		y -= s.y;
		z -= s.z;
		return vector3D(x, y, z);
	};
	//The dot product.
	float operator * (vector3D s) {
		float dot = (x*s.x) + (y*s.y) + (z*s.z);

		return dot;
	};

	//The cross product.
	vector3D cross(vector3D a, vector3D b) {
		//((aybz - azby), (azbx-axbz), (axby-bxay))
		return vector3D(a.y*b.z - a.z*b.y,
			a.z*b.x - a.x*b.z,
			a.x*b.y - b.x*a.y);
	}

	float mag() {
		float s = sqrtf(x*x + y * y + z * z);
		return s;
	};

	String serialize() {
		String value = (String)"" + String(x, 6) + "," + String(y, 6) + "," + String(z, 6) + "|";
		return value;
	}
};