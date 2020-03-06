/*
 * Created by Noah Williams on 3/5/2020.
 * Purpose: To record ultrasound along with incline.
 */
#include <DriveTrain.h> //Deals with spinning the wheels of the rover.
#include <save.h>
#include <echo.h>
#include <orientation.h>
#include <coroutine.h>
DriveTrain drive;
orientation orient;
echo echo;
save save;
coroutine printCoroutine;
coroutine transmitEnd;
String dataRead = "";
void setup() {
  // put your setup code here, to run once:
  drive.setup();
  save.setup();
  save.writeFile("read.txt");
  echo.setup();
  orient.setup();
  printCoroutine.setup(0);
  transmitEnd.setup(6*10000); //One minute
   drive.stop();
}
void loop() {
  // put your main code here, to run repeatedly:
  transmitEnd.loop();
  printCoroutine.loop();
  orient.loop();
  echo.loop();
 if(printCoroutine.readyState){
  dataRead = "";
  dataRead += "echo distance: ";
  dataRead += echo.distance;
  dataRead += " || incline: ";
  dataRead += orient.incline;
  int str_len = dataRead.length() + 1; 
  char data[str_len]; 
  dataRead.toCharArray(data, str_len);
  save.saveToFileln(data);
  printCoroutine.reset();
  Serial.println(orient.incline);
  Serial.println(echo.distance);
 }

 if(transmitEnd.readyState){
    save.closeFile();
    delay(1000000);
 }

 drive.forward(25);
 delay(10);
}
