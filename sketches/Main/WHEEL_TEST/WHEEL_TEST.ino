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
      delay(1000);
      forwards(-30);
      delay(1000);
      forwards(30);
      spin(80, LEFTTOP | RIGHTTOP | LEFTBOTTOM | RIGHTBOTTOM);
      delay(1000);
      spin(-80, LEFTTOP | RIGHTTOP | LEFTBOTTOM | RIGHTBOTTOM);
  }


}


void forwards(int speed) {
  drive.forward(speed);
}


void spin(int r, int motors) {
  drive.turn(r, motors);
}

void reset() {
  spin(0, LEFTTOP | RIGHTTOP | LEFTBOTTOM | RIGHTBOTTOM);
}
