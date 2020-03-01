#include <DriveTrain.h> //Deals with spinning the wheels of the rover.
#include <kalmanFilter.h> //Gets IMU device for the rover
#include "coroutine.h"
#include <echo.h>

kalmanFilter kalman; //A common library for IMU devices.
DriveTrain drive;
coroutine driveCoroutine;
coroutine kalmanCoroutine;
coroutine avoid;
coroutine printData;
enum Rover {
  TRACK,
  BACKUP,
  AVOID,
  SUCCESS,
};

Rover machine;
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
  printData.setup(1500);
  avoid.setup(2000);
  drive.setup();
  kalman.setup();
  echo.setup();
  reset();
  forwards(0);
  calibrate();
  kalman.roverGPS.setDestinations(40.176001, -75.274005, 0); //First tree south tree.
  //kalman.roverGPS.setDestinations(40.176103, -75.273777, 1); //Second east tree.
  //kalman.roverGPS.setDestinations(40.176275, -75.274011, 2); //Third starting point.
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
    drive.spin(50);
  }

  reset();
  Serial.println(""); Serial.println("Calibrated");
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
 kalmanCoroutine.loop();
 driveCoroutine.loop();
 avoid.loop();
 printData.loop();
 if(kalmanCoroutine.readyState){
   echo.loop();
   tolerance = kalman.tolerance;//Let's play with different values.
   heading = kalman.orient.heading; //Kalman filters it for you.
   bearing = kalman.roverGPS.bearing; //But you can just call roverGPS (no filter).
   difference = abs(heading-bearing);
   kalman.loop();
     if(kalman.destinationReached()){
        machine = SUCCESS;
     }
  }
  if(printData.readyState){
    serializeData();
  }
  delay(10);
  if(echo.distance <= 50){
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
    case SUCCESS:
    haltRover();
    break;
  }

 kalmanCoroutine.reset();
 driveCoroutine.reset();
 avoid.reset();
 printData.reset();
}

void backup(){
  reset();
  forwards(90);
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
  if (abs(heading-bearing) < tolerance){
     reset();
  }else{
    spin(wheelDirection);
  }
}

void forwards(int speed) {
  drive.forward(speed);
}

void spin(int r) {
  drive.turn(r);
}

void reset() {
  drive.stop();
}

void haltRover(){
  if(kalman.destinationReached()){
      drive.stop();
      drive.forward(0);
  }else{
    machine = TRACK;
  }

}

void serializeData(){
     tolerance = kalman.tolerance;//Let's play with different values.
   heading = kalman.orient.heading; //Kalman filters it for you.
   bearing = kalman.roverGPS.bearing; //But you can just call roverGPS (no filter).
   difference = abs(heading-bearing);
   kalman.roverGPS.serialize();
   /*
     Serial.print("Heading: ");
     Serial.print(heading);
     Serial.print(" ||  Bearing: ");
     Serial.println(bearing);
     Serial.print("DISTANCE: ");
     Serial.println(kalman.roverGPS.distance);
     Serial.println(machine);
     Serial.println(echo.distance);
     Serial.print("LOCATION: ");
     Serial.print(String(kalman.roverGPS.position.x(), 8));
     Serial.print(" ");
     Serial.println(String(kalman.roverGPS.position.y(), 8));
     Serial.print("GPS RAW: ");
     Serial.print(String(kalman.roverGPS.gps.latitudeDegrees, 8));
     Serial.println(String(kalman.roverGPS.gps.longitudeDegrees, 8));
     */
}
