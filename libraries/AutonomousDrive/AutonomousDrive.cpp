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
  saveData.setup(60000);
  drive.setup();
  kalman.setup();
  if(SDRecord)
    saveSD.setup();
  echosensorLeft.trigPin = LeftTrigPin;
  echosensorLeft.receivePin = LeftRecievePin;
  echosensorRight.trigPin = RightTrigPin;
  echosensorRight.receivePin = RightRecievePin;

  echosensorRight.setup();
  echosensorLeft.setup();
  SD.remove("RTD.txt");
  saveSD.writeFile("RTD.txt");
  reset();
  forwards(0);
  calibrate();
  //kalman.roverGPS.setDestinations(40.176001, -75.274005, 0); //First tree south tree.
  //kalman.roverGPS.setDestinations(40.176103, -75.273777, 1); //Second east tree.
  //kalman.roverGPS.setDestinations(40.176275, -75.274011, 2); //Third starting point.
  kalman.roverGPS.traverseDestination();
}

void AutonomousDrive::setup(bool IsManual) {
  // put your setup code here, to run once: 
  kalmanCoroutine.setup(10);
  driveCoroutine.setup(1000);
  printData.setup(1000);
  avoid.setup(2000);
  saveData.setup(60000);
  drive.setup();
  kalman.setup();
  if(SDRecord)
    saveSD.setup();
  echosensorLeft.trigPin = LeftTrigPin;
  echosensorLeft.receivePin = LeftRecievePin;
  echosensorRight.trigPin = RightTrigPin;
  echosensorRight.receivePin = RightRecievePin;
  echosensorRight.setup();
  echosensorLeft.setup();
  SD.remove("RTD.txt");
  saveSD.writeFile("RTD.txt");
  reset();
  forwards(0);
  calibrate();
}

void AutonomousDrive::calibrate(){
  forwards(25);
  uint8_t system, gyro, accel, mag = 0;
  while (system != 3)
  {
    kalman.orient.bno.getCalibration(&system, &gyro, &accel, &mag);
    Serial.print("CALIBRATING PLEASE SPIN AROUND!   sys=");
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
  forwards(0);
  Serial.println(""); Serial.println("Calibration Done. Please place Rover down.");
  delay(5000);
}

void AutonomousDrive::loop() {
  // put your main code here, to run repeatedly:
 kalmanCoroutine.loop();
 driveCoroutine.loop();
 avoid.loop();
 saveData.loop();
 printData.loop();
 kalman.roverGPS.loop();
 if(kalmanCoroutine.readyState){
    kalmanCoroutine.reset();
   kalman.loop();

    if(objectDetection){
         echosensorRight.loop();
         echosensorLeft.loop();
    }

   tolerance = kalman.tolerance;
   heading = kalman.orient.heading;
   bearing = kalman.roverGPS.bearing;
   difference = abs(heading-bearing);


   
     if(kalman.destinationReached()){
        machine = SUCCESS;
     }
  }
  if(SerializeDataEnabled){
    if(printData.readyState){
      serializeData();
    }
  }

  if(objectDetection){
    if(echosensorLeft.distance <= detectionRange || echosensorRight.distance <= detectionRange){
      machine = BACKUP;
    }
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
    case TURNINPLACE:
    turnInPlace();
    break;
  }


 driveCoroutine.reset();
 avoid.reset();
 printData.reset();
if(SDRecord){
 if(saveData.readyState){
   saveSD.closeFile();
   saveSD.writeFile("RTD.txt");

 }
    saveData.reset();
}

}

void AutonomousDrive::backup(){
  reset();
  forwards(-80);
  if(driveToMeter(4)){
    machine = TURNINPLACE;
    avoid.reset();
  }

}

void AutonomousDrive::turnInPlace(){
    forwards(0);
    if(drive.zeroRadiusTurn(90, kalman.orient)){
      machine = AVOID;
    }
}

void AutonomousDrive::avoidObstacle(){

  if(driveToMeter(6)){
    machine = TRACK;
    avoid.reset();
  }

  
}

bool AutonomousDrive::driveToMeter(float meter){
  if(kalman.orient.distanceTraveled > meter){
    forwards(0);
    kalman.orient.resetPositions();
    reset();
    return true;
  }
  return false;
}

//Determines which direction wheels will move in during turning.
void AutonomousDrive::followBearing(){
    float angle1 = ((bearing - heading) / 180) * 100;
    float angle2 = (((bearing+360) - heading) / 180) * 100;
    float angle3 = (((bearing - 360) - heading) / 180) * 100;
    if (abs(angle1) < abs(angle2) && abs(angle1) < abs(angle3))
    {
        wheelDirection = angle1;
    }
    else if (abs(angle2) < abs(angle3) && abs(angle2) < abs(angle3))
    {
        wheelDirection = angle2;
    }
    else
    {
        wheelDirection = angle3;
    }
    //Clamps the values.
    if(wheelDirection > 90){
      wheelDirection = 90;
    }else if(wheelDirection < -90){
      wheelDirection = -90;
    }
  //wheelDirection = ((-heading + bearing)/180) *100;
  
  /*
  float wheelDirection = ((180 - abs(abs(bearing-heading)-180))/180) *100; //The magnitude. Decides how much to turn.
  float diff = heading- bearing;
  if(diff < 0) //Determines the shortest angle to turn to.
     diff += 360;
  if(diff > 180)
     wheelDirection *= -1;// left turn
  */
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
     Serial.print("HOUR:MINUTES:SECONDS | ");
     Serial.print(kalman.roverGPS.gps.hour);
     Serial.print(":");
     Serial.print(kalman.roverGPS.gps.minute);
     Serial.print(":");
     Serial.println(kalman.roverGPS.gps.seconds);
     Serial.print("Heading: ");
     Serial.print(heading);
     Serial.print(" ||  Bearing: ");
     Serial.println(bearing);
     Serial.print("DESTINATION DISTANCE: ");
     Serial.println(kalman.roverGPS.distance);
     Serial.print("OBSTACLE DISTANCE: RIGHT: ");
     Serial.print(echosensorRight.distance);
     Serial.print(" | LEFT:  ");
     Serial.println(echosensorLeft.distance);
     Serial.print("LOCATION: ");
     Serial.print(String(kalman.roverGPS.position.x(), 8));
     Serial.print(",");
     Serial.println(String(kalman.roverGPS.position.y(), 8));
     Serial.print("DESTINATION: ");
     Serial.print(String(kalman.roverGPS.destination.x(), 8));
     Serial.print(",");
     Serial.println(String(kalman.roverGPS.destination.y(), 8));

          Serial.print("CURRENT STATE: ");
       switch(machine){

        case BACKUP:
        Serial.println("BACKING UP");
        break;
        case TRACK:
        Serial.println("GOING TOWARDS DESTINATION");
        break;
        case AVOID:
        Serial.println("OBSTACLE AVOIDING!");
        break;
        case SUCCESS:
        Serial.println("DESTINATION FOUND");
        break;
        case TURNINPLACE:
        Serial.println("TURNING IN PLACE.");
        break;
      }
      Serial.print("Direction Wheels Turning: ");
      Serial.println(wheelDirection);
     Serial.println("");
    Serial.print("MAGNITUDE TRAVELED: ");
     Serial.println(String(kalman.orient.distanceTraveled, 8));
     

     if(SDRecord){
      String str = "TIME: " +String(kalman.roverGPS.gps.hour) + ":"+ String(kalman.roverGPS.gps.minute) + ":"+  String(kalman.roverGPS.gps.seconds);
      int str_len = str.length() + 1; 
     char stringarray[str_len]; 
     str.toCharArray(stringarray, str_len);
     saveSD.saveToFileln(stringarray);

      //Location and destination: 
     str = "LOCATION: " +String(kalman.roverGPS.position.x(), 8) + ","+ String(kalman.roverGPS.position.y(), 8) + " | DESTINATION: "+  String(kalman.roverGPS.destination.x(), 8) + "," + String(kalman.roverGPS.destination.y(), 8);
     str_len = str.length() + 1; 
     char stringarray2[str_len]; 
     str.toCharArray(stringarray2, str_len);
     saveSD.saveToFileln(stringarray2);

     //Heading and Bearing:
     str = "HEADING|BEARING: " +String(heading) + " | "+ String(bearing);
     str_len = str.length() + 1; 
     char stringarray3[str_len]; 
     str.toCharArray(stringarray3, str_len);
     saveSD.saveToFileln(stringarray3);
     }

}

