#include <orientation.h>
#include <DriveTrain.h>

DriveTrain drive;

orientation orient;
void setup() {
  // put your setup code here, to run once:
  orient.setup();
  drive.setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  orient.loop();
  float heading = orient.heading;
  float bearing = 0;
  int wheelDirection = abs(((180 - abs(abs(bearing-heading)-180))/180)) *100; //The magnitude. Decides how much to turn.
  
  float diff = heading- bearing;
     if(diff < 0){
      
     }
     if(diff > 180 && wheelDirection > 0 && wheelDirection < 30 ){
        wheelDirection *= -1;
     }
  Serial.print("HEADING: ");
  Serial.println(heading);
  Serial.println(wheelDirection);

 // drive.forward(60);
  drive.turn(wheelDirection,60, LEFTTOP | RIGHTTOP | LEFTBOTTOM | RIGHTBOTTOM);
  
  delay(100);
}
