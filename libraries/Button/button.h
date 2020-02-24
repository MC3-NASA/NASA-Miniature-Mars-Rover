#pragma once
//Created by Noah Williams on 9/29/2019.
//Echo location functions used for finding objects via sound.
#include <Arduino.h>
using namespace std;
class button {
public:
	//Public variable access specifiers.
	int buttonPin = 2;     // the number of the pushbutton pin
	// variables will change:
	int buttonState = 0;         // variable for reading the pushbutton status

	void setup(int pin);
	void loop();
	bool checkButton();
};

