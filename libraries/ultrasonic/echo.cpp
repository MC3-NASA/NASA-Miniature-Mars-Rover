//Created by Noah Williams on 9/7/2019.
//Echo location functions used for finding objects via sound.
#include "echo.h"

void echo :: setup() {
	//Inputs/Outputs
	pinMode(trigPin, OUTPUT);
	pinMode(receivePin, INPUT);
	recieveCoro.setup(length*2);
	pulseCoro.setup(length);

}

void echo :: setup(int trig, int rec) {
	//Inputs/Outputs
	trigPin = trig;
	receivePin = rec;
	pinMode(trigPin, OUTPUT);
	pinMode(receivePin, INPUT);

}
void echo :: loop() {
	pulseCoro.loop();
	recieveCoro.loop();
	pulsate(length); //Give a high then low pulse to create sound.
	pinMode(receivePin, INPUT);
	//Converts speed to distance
	distance = (duration*0.5f)*0.0343642611683849f; //Approximate value for speed of sound.
	distance = constrain(distance, 0, 1000);
}
void echo :: pulsate(float length) {
	if(newPulse == true)
	{
		digitalWrite(trigPin, LOW);
		newPulse = false;
	}
	if(pulseCoro.readyState)
		digitalWrite(trigPin, HIGH);
	if(recieveCoro.readyState){
		newPulse = true;
		digitalWrite(trigPin, LOW);
		duration = pulseIn(receivePin, HIGH); //reads reciever. Sends back length in microseconds.
	}

	pulseCoro.reset();
	recieveCoro.reset();

}
String echo::serialize() {
	return (String)"d_e"+String(distance, 6);
}