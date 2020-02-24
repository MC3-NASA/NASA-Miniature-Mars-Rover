#pragma once
//Created by Noah Williams on 9/7/2019.
//Echo location functions used for finding objects via sound.
#include <Arduino.h>
using namespace std;
class echo {
public:
	//Public variable access specifiers.
	double duration, distance; //seconds, centimeters.
	float length = 5; //How long the wavelength should be. Micro-Seconds.
	float samples[5] = {0};
	int sampleIndex = 0;
	int sampleLength = 4;
	const int trigPin = 7, receivePin = 8;
	void setup();
	void loop();
	void pulsate(float speed);
	String serialize();
};

