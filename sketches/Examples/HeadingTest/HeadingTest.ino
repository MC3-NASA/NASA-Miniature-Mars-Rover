#include <orientation.h>
orientation orient;
void setup() {
  // put your setup code here, to run once:
  orient.setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  orient.loop();
  orient.serialize();
  delay(100);
}
