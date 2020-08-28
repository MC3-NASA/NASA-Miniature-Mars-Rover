/*
 * Created by Noah Williams on 3/21/2020.
 * 
 * Purpose: A very simple wheel test that drives straight, then backwards, then turns right, the turns left.
 * 
 * Please note this does not use an IMU. So a pattern will not be found! The IMU is needed in order to trace a recognizable consistent pattern.
 */
#include <DriveTrain.h> //Deals with spinning the wheels of the rover.
DriveTrain drive;
/*
 * VARIABLES YOU CAN CHANGE!!!!!!
 * 
 */

int SPEED = 30; //SET THIS TO 0 TO TEST STRAIGHT WHEELS!!!



void setup() {
  // put your setup code here, to run once:
  drive.setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  reset();
  if(SPEED > 0){
      forwards(30);
      delay(100);
      serializeData(0);
      delay(5000);
      forwards(-30);
      serializeData(0);
      delay(5000);
      forwards(100);
      spin(80, LEFTTOP | RIGHTTOP | LEFTBOTTOM | RIGHTBOTTOM);
      serializeData(80);
      delay(5000);
      forwards(-50);
      spin(-80, LEFTTOP | RIGHTTOP | LEFTBOTTOM | RIGHTBOTTOM);
      serializeData(-80);
      delay(5000);
      
  }


}


void forwards(float speed) {
  drive.forward(speed);
}


void spin(int r, int motors) {
  drive.turn(r, motors);
}

void reset() {
  spin(0, LEFTTOP | RIGHTTOP | LEFTBOTTOM | RIGHTBOTTOM);
}

void serializeData(int angle){
  Serial.print("Turning this angle: ");
  Serial.println(angle);
  String str = "V1: "+String(drive.V1) + " | | V2: "+String(drive.V2) + " | | V3: "+String(drive.V3) + " | | V4: "+String(drive.V4) + " | | V5: "+String(drive.V5) + " | | V6: "+String(drive.V6);
  Serial.println(str);
}
