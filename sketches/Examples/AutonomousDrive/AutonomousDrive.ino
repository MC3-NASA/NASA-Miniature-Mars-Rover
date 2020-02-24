#include <DriveTrain.h> //Deals with spinning the wheels of the rover.
#include <kalmanFilter.h> //Gets IMU device for the rover
#include "coroutine.h"
#include <echo.h>

kalmanFilter kalman; //A common library for IMU devices.
DriveTrain drive;
coroutine driveCoroutine;
coroutine kalmanCoroutine;
coroutine avoid;
enum State {
  TRACK,
  BACKUP,
  AVOID,
};

State machine;
echo echo;

 double tolerance = kalman.tolerance;//Let's play with different values.
 double heading = kalman.orient.heading; //Kalman filters it for you.
 double bearing = kalman.roverGPS.bearing; //But you can just call roverGPS (no filter).
 double difference = abs(heading-bearing);
 int wheelDirection = 0;

void setup() {
  // put your setup code here, to run once: 
  kalmanCoroutine.setup(10);
  driveCoroutine.setup(1000);
  avoid.setup(2000);
  drive.setup();
  kalman.setup();
  echo.setup();
  reset();
  forwards(0);
  calibrate();
}

void calibrate(){
  forwards(55);
  uint8_t system, gyro, accel, mag = 0;
  while (system != 3)
  {
    kalman.orient.bno.getCalibration(&system, &gyro, &accel, &mag);
    Serial.print("CALIBRATION: Sys=");
    Serial.print(system, DEC);
    Serial.print(" Gyro=");
    Serial.print(gyro, DEC);
    Serial.print(" Accel=");
    Serial.print(accel, DEC);
    Serial.print(" Mag=");
    Serial.println(mag, DEC);
    delay(100);
    Leftspin();
    forwards(55);
  }

  reset();
  forwards(0);
  Serial.println(""); Serial.println("Calibrated");
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
 kalmanCoroutine.loop();
 driveCoroutine.loop();
 avoid.loop();
 if(kalmanCoroutine.readyState){
   echo.loop();
   tolerance = kalman.tolerance;//Let's play with different values.
   heading = kalman.orient.heading; //Kalman filters it for you.
   bearing = kalman.roverGPS.bearing; //But you can just call roverGPS (no filter).
   difference = abs(heading-bearing);
     Serial.print("Heading: ");
     Serial.print(heading);
     Serial.print(" ||  Bearing: ");
     Serial.println(bearing);
     kalman.loop();
  }
 delay(10);

  if(echo.distance <= 80){
    machine = BACKUP;
  }

  switch(machine){

    case BACKUP:
    backup();
    break;
    case TRACK:
    followBearing();
    break;
    case AVOID:
    avoidObstacle();
    break;
  }

 kalmanCoroutine.reset();
 driveCoroutine.reset();
 avoid.reset();
}

void backup(){
  reset();
  forwards(-90);
   avoid.loop();
  if(avoid.readyState){
    machine = AVOID;
  }
  avoid.reset();
}

void avoidObstacle(){
  forwards(60);
  spin(50);
  avoid.loop();
  if(avoid.readyState){
    machine = TRACK;
  }
  avoid.reset();
  
}

void followBearing(){
  forwards(90);
  wheelDirection = ((heading - bearing)/180) *100;
  Serial.println(wheelDirection);
  if (abs(heading-bearing) < tolerance){
     reset();
  }else{
    spin(wheelDirection);
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

void spin(int r) {

  // Front
  drive.moveTo(23, r);
  drive.moveTo(29, r);

  // Back
  drive.moveTo(24, -r);
  drive.moveTo(26, -r);
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
