#include <orientation.h>
orientation rover = orientation();
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  rover.setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  rover.loop();
  delay(100);
}
