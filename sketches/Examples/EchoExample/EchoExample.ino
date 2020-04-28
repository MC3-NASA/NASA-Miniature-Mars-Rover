#include <echo.h>
echo echo1;
echo echo2;
void setup() {
  // put your setup code here, to run once:
    echo2.trigPin = 9;
  echo2.receivePin = 10;
  echo1.setup();
  echo2.setup();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  echo1.loop();
  echo2.loop();
  Serial.print("Echo 1: ");
  Serial.println(echo1.distance);
  Serial.print("Echo 2: ");
  Serial.println(echo2.distance);

}
