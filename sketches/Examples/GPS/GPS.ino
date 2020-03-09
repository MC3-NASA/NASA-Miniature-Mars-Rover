#include <RoverGPS.h>
#include "coroutine.h"
RoverGPS gps;
coroutine printCoroutine;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  gps.setup();
  printCoroutine.setup(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  gps.loop();
  printCoroutine.loop();
  if(printCoroutine.readyState){
    gps.serialize();
    printCoroutine.reset();
  }
}
