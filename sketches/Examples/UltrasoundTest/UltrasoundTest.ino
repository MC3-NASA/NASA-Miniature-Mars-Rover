#include <DriveTrain.h> //Deals with spinning the wheels of the rover.
#include "coroutine.h"
#include <kalmanFilter.h>
#include <echo.h>
//kalmanFilter kalman;
orientation orient;
DriveTrain drive;
echo echo;
coroutine driveCoroutine;
float rposx = -99999;
float rposy = -99999;
float rposz = -99999;
boolean moveback = false;
void setup() {
  // put your setup code here, to run once:
  driveCoroutine.setup(1000);
  drive.setup();
  orient.setup();
  echo.setup();
  Serial.begin(9600);
  reset();
}

void loop() {
  orient.loop();
  echo.loop();
  
  if (!moveback)
  {
    forwards(100);
  }
  else
  {
    forwards(-100);
  }

  if (echo.distance < 1)
  {
    moveback = true;
    rposx = orient.position.x();
    rposy = orient.position.y();
    rposz = orient.position.z();
  }
  while (!(abs(sqrt(rposx*rposx+rposy*rposy+rposz*rposz) - orient.computeMagnitude()) < 1))
  {
    moveback = false;
  }
  delay(10);
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
