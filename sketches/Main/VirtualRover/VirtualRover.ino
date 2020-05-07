/*
 * Created by Noah Williams.
 * Purpose: Loads waypoints given by Unity. This is used for VirtualRover
 * 
 * No setup required. This will automatically set points and give information if that point is reachable.
 */
 
 #include <SerialCommand.h>
 #include <save.h>
 #include <Map.h>
 #include <AutonomousDrive.h>
/*
 * LIST OF VARIABLES THAT CAN BE CHANGED:
 */

//THESE VARIABLES ARE TO BE CHANGED BY USER.
int detectionRange = 40; //CENTIMETERS. DETERMINES HOW FAR OBJECT CAN BE DETECTED BEFORE TURNING.
float accuracyRadius = 5; //How close to destination before succeeding. IN METERS. 

//Changes the pins of the ultrasound:
int LeftTrigPin = 7;
int LeftRecievePin = 8;
int RightTrigPin = 9;
int RightRecievePin = 10;

bool ObjectDetectionEnabled = true; //Turns on or off obstacle avoidance.


bool SDRecordingEnabled = false; //Turns on or off SD card recordings. NOTE: you need an SD card, otherwise turn it off!

bool SerializeDataEnabled = false; //Turns on or off data in the serial monitor. Note: turning off will improve performance. THIS NEEDS TO BE ON FOR SD CARD RECORDINGS.


//Do not touch below
AutonomousDrive autoDrive;
 SerialCommand sCmd; //Get serial commands from Virtual Rover (Unity)
 save save;
 Map internalMap;
 RoverGPS roverGPS;
 coroutine printCoroutine;
 int loadingMap = 1; //Determines if loading map. 0 no load, 1 is loading.
 float centralLat = -1;
 float centralLon = -1;
 float dests[16];
 int readyToDrive = 0;


enum BootLoads {
        UNITY,
        DRIVE,
        SETUPDRIVE,
};

BootLoads bootloader = UNITY;
void setup() {
  roverGPS.setup();
  printCoroutine.setup(3000);
  // put your setup code here, to run once:
  sCmd.addCommand("W#W", WayPointHandler); //Command when recieved a list of way points.
  sCmd.addCommand("L#L", LatLonHandler); //Command when recieved a list of way points.
  sCmd.addCommand("S#S", stopHandler); //Stops recieving points.
  sCmd.addCommand("D#D", driveHandler); //Goes into Drive
}

void loop() {
  switch(bootloader){

    case UNITY:
    updateSerialCommunicator();
    break;
    case DRIVE:
    autoDrive.loop();
    break;
    case SETUPDRIVE:
    setupAutoDrive();
    break;
  }

}

void setupAutoDrive(){
  bootloader = DRIVE;
  for(int i = 0; i <= 10; i++){
     autoDrive.kalman.destinations[i] = (double)dests[i];
  }

  autoDrive.detectionRange = detectionRange;
    autoDrive.objectDetection = ObjectDetectionEnabled;
  autoDrive.LeftTrigPin = LeftTrigPin;
  autoDrive.LeftRecievePin = LeftRecievePin;
  autoDrive.RightTrigPin = RightTrigPin;
  autoDrive.RightRecievePin = RightRecievePin;
   autoDrive.kalman.destinationRadius = accuracyRadius;

   
  autoDrive.SDRecord = SDRecordingEnabled;
  autoDrive.SerializeDataEnabled = SerializeDataEnabled;
  autoDrive.setup();
}


void updateSerialCommunicator(){
  roverGPS.loop();
  printCoroutine.loop();
  if(printCoroutine.readyState){
    //42.299683, 12.984426
      String value = "LOC" + String(roverGPS.position.x(), 15) + "," + String(roverGPS.position.y(), 15);
      //String value = "LOC" + String(42.299683, 15) + "," + String(12.984426, 15);
      Serial.println(value);
       if (Serial.available() > 0)
       {
          sCmd.readSerial();
       }
       for(int i = 0; i < 16; i++){
        Serial.print(String(dests[i], 15));
        Serial.print(", ");
        i++;
        Serial.println(String(dests[i], 15));
        
      }
      printCoroutine.reset();
  }
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
  //Serial.println("W_W");
  internalMap.latitude = centralLat;
  internalMap.longitude = centralLon;
  delay(10);
}

//Gets the x,y of the pixel data to be processed by mapProjection math.
//This is handled after central lat and lon are set.
void WayPointHandler() {
  char *arg;
  arg = sCmd.next();
  if (arg != NULL)
  {
    String processStr = "";
    processStr += String(arg);
    //Ensures Central Lat/Lon has recieved data.
    if(abs(centralLat) >= 0 && abs(centralLon) >= 0){
      int indexS = 0; //Index of the string.
      int x = 0;
      int y = 0;

      int i = 0; //Index of destinations set.
      if(processStr[indexS] == 'A') //Then this is first array of destinations.
      {
       i = 0; //Index of destinations set.
      }else{ //Then this is second.
        i = 8;
      }
      //Set the destinations:
      while(processStr[indexS] != '\0'){
        indexS++;
        String buff = ""; //Used to store the string temporarily.
        //Until we reach a comma store values.
        while(processStr[indexS] != ',' && processStr[indexS] != '\0'){
          buff += processStr[indexS];
          indexS++;
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
  if( bootloader == UNITY){
  loadingMap = 1;
  bootloader = SETUPDRIVE;
  Serial.println("S_S");
}

}

void driveHandler(){
  if(bootloader == UNITY){
     bootloader = SETUPDRIVE;
  }
}
