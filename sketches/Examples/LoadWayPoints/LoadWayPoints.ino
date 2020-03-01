/*
 * Created by Noah Williams.
 * Purpose: Loads waypoints given by Unity. This is an isolated example to be used for testing purposes.
 */
 #include <SerialCommand.h>
 #include <save.h>
 SerialCommand sCmd; //Get serial commands from Virtual Rover (Unity)
 save save;
 int loadingMap = 1; //Determines if loading map. 0 no load, 1 is loading.
 float centralLat = 0;
 float centralLon = 0;
void setup() {
  // put your setup code here, to run once:
  sCmd.addCommand("W#W", WayPointHandler); //Command when recieved a list of way points.
  sCmd.addCommand("L#L", LatLonHandler); //Command when recieved a list of way points.
  sCmd.addCommand("S#S", stopHandler); //Stops recieving points.
  /*
  while(loadingMap >0){
    sCmd.readSerial();
  }
  */
}

void loop() {
  // put your main code here, to run repeatedly:
   if (Serial.available() > 0)
   {
      sCmd.readSerial();
   }
   Serial.println(centralLat);
   Serial.println(centralLon);
   delay(200);
}

void WayPointHandler () {
  char *arg;
  arg = sCmd.next();
  if (arg != NULL)
  {
    Serial.println(arg);
  }
  else
    Serial.println("no message");
  delay(10);
}

void LatLonHandler () {
  char *arg;
  arg = sCmd.next();
  if (arg != NULL)
  {
      int indexS = 0;
      indexS=save.processString(String(arg), &centralLat, indexS);
      save.processString(String(arg), &centralLon, indexS);
  }
  Serial.println("S#S");
  delay(10);
}

void stopHandler () {
  loadingMap = 0;
}
