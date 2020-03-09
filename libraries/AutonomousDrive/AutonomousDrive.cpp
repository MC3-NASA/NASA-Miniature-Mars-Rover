/*
 * Created by Noah Williasm
 */
#include "AutonomousDrive.h"
void AutonomousDrive::setup() {
  // put your setup code here, to run once: 
  kalmanCoroutine.setup(10);
  driveCoroutine.setup(1000);
  printData.setup(1500);
  avoid.setup(2000);
  drive.setup();
  kalman.setup();
  echosensor.setup();
  reset();
  forwards(0);
  calibrate();
  //kalman.roverGPS.setDestinations(40.176001, -75.274005, 0); //First tree south tree.
  //kalman.roverGPS.setDestinations(40.176103, -75.273777, 1); //Second east tree.
  //kalman.roverGPS.setDestinations(40.176275, -75.274011, 2); //Third starting point.
  kalman.roverGPS.traverseDestination();
}

void AutonomousDrive::calibrate(){
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

void AutonomousDrive::loop() {
  // put your main code here, to run repeatedly:
 kalmanCoroutine.loop();
 driveCoroutine.loop();
 avoid.loop();
 printData.loop();
 kalman.roverGPS.loop();
 if(kalmanCoroutine.readyState){
   kalman.loop();
   echosensor.loop();
   tolerance = kalman.tolerance;//Let's play with different values.
   heading = kalman.orient.heading; //Kalman filters it for you.
   bearing = kalman.roverGPS.bearing; //But you can just call roverGPS (no filter).
   difference = abs(heading-bearing);

 kalmanCoroutine.reset();
     if(kalman.destinationReached()){
        machine = SUCCESS;
     }
  }
  if(printData.readyState){
    serializeData();
  }
  if(echosensor.distance <= 10){
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


 driveCoroutine.reset();
 avoid.reset();
 printData.reset();
}

void AutonomousDrive::backup(){
  reset();
  forwards(-80);
   avoid.loop();
  if(avoid.readyState){
    machine = AVOID;
  }
  avoid.reset();
}

void AutonomousDrive::avoidObstacle(){
  forwards(80);
  spin(50, LEFTTOP | RIGHTTOP | LEFTBOTTOM | RIGHTBOTTOM);
  avoid.loop();
  if(avoid.readyState){
    machine = TRACK;
  }
  avoid.reset();
  
}

void AutonomousDrive::followBearing(){
  //wheelDirection = ((-heading + bearing)/180) *100;
  float wheelDirection = ((180 - abs(abs(bearing-heading)-180))/180) *100; //The magnitude. Decides how much to turn.
  float diff = heading- bearing;
  if(diff < 0) //Determines the shortest angle to turn to.
     diff += 360;
  if(diff > 180)
     wheelDirection *= -1; // left turn
          
  if (abs(heading-bearing) < tolerance){
     reset();
  }else{
    forwards(90);
    spin(wheelDirection, LEFTTOP | RIGHTTOP | LEFTBOTTOM | RIGHTBOTTOM);
  }
}

void AutonomousDrive::forwards(int speed) {
  drive.forward(speed);
}

void AutonomousDrive::spin(int r, int motors) {
  drive.turn(r, motors);
}

void AutonomousDrive::reset() {
  spin(0, LEFTTOP | RIGHTTOP | LEFTBOTTOM | RIGHTBOTTOM);
}

void AutonomousDrive::haltRover(){
  if(kalman.destinationReached()){
      drive.stop();
      drive.forward(0);
  }else{
    machine = TRACK;
  }

}

void AutonomousDrive::serializeData(){
     tolerance = kalman.tolerance;//Let's play with different values.
   heading = kalman.orient.heading; //Kalman filters it for you.
   bearing = kalman.roverGPS.bearing; //But you can just call roverGPS (no filter).
   difference = abs(heading-bearing);
   //kalman.roverGPS.serialize();
     Serial.print("Heading: ");
     Serial.print(heading);
     Serial.print(" ||  Bearing: ");
     Serial.println(bearing);
     Serial.print("DISTANCE: ");
     Serial.println(kalman.roverGPS.distance);
     Serial.println(machine);
     Serial.println(echosensor.distance);
     Serial.print("LOCATION: ");
     Serial.print(String(kalman.roverGPS.position.x(), 8));
     Serial.print(" ");
     Serial.println(String(kalman.roverGPS.position.y(), 8));
     Serial.print("DESTINATION: ");
     Serial.print(String(kalman.roverGPS.destination.x(), 8));
     Serial.print(" ");
     Serial.println(String(kalman.roverGPS.destination.y(), 8));
     Serial.print("GPS RAW: ");
     Serial.print(String(kalman.roverGPS.gps.latitudeDegrees, 8));
     Serial.println(String(kalman.roverGPS.gps.longitudeDegrees, 8));
}