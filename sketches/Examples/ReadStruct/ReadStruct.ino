#include <save.h>
save save;
void setup() {
  // put your setup code here, to run once:
  save.setup();
  save.read_STRUCT("CCCA");
}

void loop() {
  // put your main code here, to run repeatedly:

  delay(100);
}
