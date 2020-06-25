#include <echo.h>
echo echo1;
echo echo2;
void setup() {

  ///YOU CAN CHANGE THE UPDATE RATE HERE:
  //NOTE, IT SHOULD STILL PRINT RAPIDLY, BUT ONLY UPDATE ACCORDING TO THIS NUMBER. MILLISECONDS:
  echo1.length = 100;
  echo2.length = 100;

  ////////////
  echo2.trigPin = 4;
  echo2.receivePin = 3;
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
