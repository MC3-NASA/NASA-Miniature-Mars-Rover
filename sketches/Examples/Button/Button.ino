#include <button.h>
button button;
void setup() {
  button.setup(2); //Set the button to digital pin 2.
}

void loop() {
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (button.checkButton()) {
    Serial.println("LED ON");
  } else {
    Serial.println("LED OFF");
  }
}
