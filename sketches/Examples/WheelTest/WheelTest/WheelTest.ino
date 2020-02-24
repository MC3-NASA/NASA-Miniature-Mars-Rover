#include <DriveTrain.h> //Deals with spinning the wheels of the rover.
#include "coroutine.h"

DriveTrain drive;
coroutine driveCoroutine;

void setup() {
  // put your setup code here, to run once:
  driveCoroutine.setup(1000);
  drive.setup();
  reset();
  forwards(50);
  
}

void loop() {
  // put your main code here, to run repeatedly:
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
