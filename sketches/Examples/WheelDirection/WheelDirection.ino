#include <orientation.h>
orientation orient;
void setup() {
  // put your setup code here, to run once:
  orient.setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  orient.loop();
  float heading = orient.heading;
  float bearing = 160;
  float wheelDirection = ((180 - abs(abs(bearing-heading)-180))/180) *100; //The magnitude. Decides how much to turn.
  float diff = heading- bearing;
     if(diff < 0)
         diff += 360;
     if(diff > 180)
          wheelDirection *= -1; // left turn
  Serial.print("HEADING: ");
  Serial.println(heading);
  Serial.println(wheelDirection);
  delay(100);
}
