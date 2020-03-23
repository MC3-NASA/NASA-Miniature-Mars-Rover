#include <echo.h>
echo echo;
void setup() {
  // put your setup code here, to run once:
  echo.setup();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  echo.loop();
  Serial.println(echo.distance);
  delay(100);

}
