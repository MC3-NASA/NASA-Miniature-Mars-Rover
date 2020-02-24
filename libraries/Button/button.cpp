#include "button.h"
void button::setup(int pin) {
	// initialize the pushbutton pin as an input:
	buttonPin = pin;
	buttonState = 0;
	pinMode(buttonPin, INPUT);
}

void button::loop() {

}

bool button :: checkButton() {
	// read the state of the pushbutton value:
	buttonState = digitalRead(buttonPin);
	// check if the pushbutton is pressed. If it is, the buttonState is HIGH:
	if (buttonState == HIGH) {
		return true;
	}
	else {
		return false;
	}
}