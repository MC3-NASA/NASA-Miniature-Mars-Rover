#include <DriveTrain.h> //Deals with spinning the wheels of the rover.
#include "coroutine.h"

DriveTrain drive;
coroutine driveCoroutine;
coroutine turnCoroutine;
boolean turnRight = false;


void setup() {
  // put your setup code here, to run once:

  turnCoroutine.setup(13000);
  driveCoroutine.setup(1000);
  drive.setup();
  reset();
  forwards(100);
  Serial.begin(9600);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  turnCoroutine.loop();
  driveCoroutine.loop();
  if(turnCoroutine.readyState){
    turnCoroutine.reset();
    if (turnRight) {
      turnRight = false;
    }
    else {
      turnRight = true;
    }
  }
  if (turnRight) {
    Rightspin();
    }
  else {
    Leftspin();
    }
  /*
  Serial.println("Voltage");
  Serial.println("FLS");
  Serial.println(drive.getVoltage(23));
  Serial.println("FLW");
  Serial.println(drive.getVoltage(25));
  Serial.println("FRS");
  Serial.println(drive.getVoltage(29));
  Serial.println("FRW");
  Serial.println(drive.getVoltage(27));
  Serial.println("MLW");
  Serial.println(drive.getVoltage(21));
  Serial.println("MRW");
  Serial.println(drive.getVoltage(22));
  Serial.println("BLS");
  Serial.println(drive.getVoltage(24));
  Serial.println("BLW");
  Serial.println(drive.getVoltage(20));
  Serial.println("BRS");
  Serial.println(drive.getVoltage(26));
  Serial.println("BRW");
  Serial.println(drive.getVoltage(28));
  Serial.println();

  Serial.println("Temperature");
  Serial.println("FLS");
  Serial.println(drive.getTemp(23));
  Serial.println("FLW");
  Serial.println(drive.getTemp(25));
  Serial.println("FRS");
  Serial.println(drive.getTemp(29));
  Serial.println("FRW");
  Serial.println(drive.getTemp(27));
  Serial.println("MLW");
  Serial.println(drive.getTemp(21));
  Serial.println("MRW");
  Serial.println(drive.getTemp(22));
  Serial.println("BLS");
  Serial.println(drive.getTemp(24));
  Serial.println("BLW");
  Serial.println(drive.getTemp(20));
  Serial.println("BRS");
  Serial.println(drive.getTemp(26));
  Serial.println("BRW");
  Serial.println(drive.getTemp(28));
  Serial.println();
  */
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
