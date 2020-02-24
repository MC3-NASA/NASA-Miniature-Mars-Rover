#include <RoverGPS.h>

RoverGPS gps;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  gps.setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  gps.loop();
}
