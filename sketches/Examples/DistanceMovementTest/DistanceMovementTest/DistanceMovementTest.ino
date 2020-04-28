#include <DriveTrain.h> //Deals with spinning the wheels of the rover.
#include "coroutine.h"
#include <kalmanFilter.h>
kalmanFilter kalman;
DriveTrain drive;
coroutine driveCoroutine;
coroutine serialCoroutine;

void setup() {
  // put your setup code here, to run once:
  driveCoroutine.setup(1000);
  serialCoroutine.setup(2000);
  drive.setup();
  kalman.setup();
  Serial.begin(9600);
  delay(1000);
  reset();
}

void loop() {
  kalman.loop();
  serialCoroutine.loop();
  if(serialCoroutine.readyState){
    kalman.orient.serialize();
    serialCoroutine.reset();
    Serial.println(kalman.orient.distanceTraveled);
  }
  reset();
  
  // put your main code here, to run repeatedly:
  //Rightspin();
  delay(10);

  if(kalman.orient.distanceTraveled > 1.5){
    forwards(0);
    Serial.println("Point Reached");
    Serial.println(kalman.orient.distanceTraveled);
    kalman.orient.resetPositions();
    reset();
  }else{
      forwards(90);
  }
}

void forwards(int speed) {
  drive.forward(speed);
}

void spin(int r, int motors) {
  drive.turn(r, motors);
}

void reset() {
  drive.stop();
}
