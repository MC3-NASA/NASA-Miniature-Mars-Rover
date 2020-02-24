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
  }
  reset();
  
  // put your main code here, to run repeatedly:
  //Rightspin();
  delay(10);

  if(kalman.orient.position.y() > 1.5){
    forwards(0);
    reset();
  }else{
      forwards(90);
  }
}

void forwards(int speed) {
  // left side
  drive.spinAt(25, speed);
  drive.spinAt(21, speed);
  drive.spinAt(20, speed);

  // right side
  drive.spinAt(27, -speed);
  drive.spinAt(22, -speed);
  drive.spinAt(28, -speed);
}

void Rightspin() {

  // Front
  drive.moveTo(23, 50);
  drive.moveTo(29, 50);

  // Back
  drive.moveTo(24, -50);
  drive.moveTo(26, -50);
}

void Leftspin() {

  // Front
  drive.moveTo(23, -50);
  drive.moveTo(29, -50);

  // Back
  drive.moveTo(24, 50);
  drive.moveTo(26, 50);
}

void reset() {
  /* Turn right */
  // front
  drive.moveTo(23, 0);
  drive.moveTo(29, 0);

  // back
  drive.moveTo(24, 0);
  drive.moveTo(26, 0);

  if(driveCoroutine.readyState){
      /* Turn Left */
    // front
    drive.moveTo(23, 0);
    drive.moveTo(29, 0);
  
    // back
    drive.moveTo(24, 0);
    drive.moveTo(26, 0);
  }
}
