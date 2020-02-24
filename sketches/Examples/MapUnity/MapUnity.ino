//Run this code to visualize map within unity.
#include <Map.h>
#include <kalmanFilter.h>
Map maps = Map(10, 1);
kalmanFilter kalman;
void setup() {
  Serial.begin(9600);
  kalman.setup();
}

void loop() {
  // put your main code here, to run repeatedly:

  kalman.loop();
  Serial.println(maps.serialize());
  delay(100);
}
