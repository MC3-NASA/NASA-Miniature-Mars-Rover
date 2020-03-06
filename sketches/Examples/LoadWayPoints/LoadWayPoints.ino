/*
 * Created by Noah Williams.
 * Purpose: Loads waypoints given by Unity. This is an isolated example to be used for testing purposes.
 */
 #include <SerialCommand.h>
 #include <save.h>
 #include <RoverGPS.h>
 #include <Map.h>
 SerialCommand sCmd; //Get serial commands from Virtual Rover (Unity)
 save save;
 Map internalMap;
 int loadingMap = 1; //Determines if loading map. 0 no load, 1 is loading.
 float centralLat = 40.176001;//-1;
 float centralLon = -75.274005;//-1;
 float dests[13];
void setup() {
  // put your setup code here, to run once:
  sCmd.addCommand("W#W", WayPointHandler); //Command when recieved a list of way points.
  sCmd.addCommand("L#L", LatLonHandler); //Command when recieved a list of way points.
  sCmd.addCommand("S#S", stopHandler); //Stops recieving points.
  internalMap.latitude = 40.176001;
  internalMap.longitude = -75.274005;
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
   for(int i = 0; i < 12; i++){
    Serial.println("");
    Serial.print(String(dests[i], 15));
    Serial.print(", ");
    i++;
    Serial.print(String(dests[i], 15));

   }
   delay(3000);

}

//When Unity Sends Lat and Lon data, this processes it into a floats.
//Gets the central Lat and Lon to be used for processing image data.
void LatLonHandler () {
  char *arg;
  arg = sCmd.next();
  if (arg != NULL)
  {
      int indexS = 0;
      indexS=save.processString(String(arg), &centralLat, indexS);
      save.processString(String(arg), &centralLon, indexS);
  }
  Serial.println("W_W");
  delay(10);
}

//Gets the x,y of the pixel data to be processed by mapProjection math.
//This is handled after central lat and lon are set.
void WayPointHandler() {
  char *arg;
  arg = sCmd.next();
  if (arg != NULL)
  {
    String processStr = "h";
    processStr += String(arg);
    //Ensures Central Lat/Lon has recieved data.
    if(abs(centralLat) >= 0 && abs(centralLon) >= 0){
      int i = 0; //Index of destinations set.
      int indexS = 0; //Index of the string.
      int x = 0;
      int y = 0;
      //Set the destinations:
      while(processStr[indexS] != '\0'){
        indexS++;
        String buff = ""; //Used to store the string temporarily.
        //Until we reach a comma store values.
        while(processStr[indexS] != ',' && processStr[indexS] != '\0'){
          buff += processStr[indexS];
          indexS++;
        }
        if(processStr[indexS] == '\0'){
          break;
        }
        //If it is the first index, then x value, if second then y value. Also set destination when y value.
        if((i%2) == 0){
          x = buff.toFloat();
        }else{
          y = buff.toFloat();
          imu::Vector<2> vec = imu::Vector<2>(-1,-1);
          vec = internalMap.getRealPosition(x, y,vec);
          dests[i-1] = vec.x();
          dests[i] = vec.y();
        }
        i++;
      } //while != '\0'
    }
  }
  Serial.println("S#S");
}

void stopHandler () {
  loadingMap = 0;
}
