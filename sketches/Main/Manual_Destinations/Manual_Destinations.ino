#include <AutonomousDrive.h>
AutonomousDrive autoDrive;
RoverGPS gps;
/*
 * Created by Noah Williams on 3/21/2020.
 * 
 * Purpose: Makes Rover drive to a manual destination. Set the latitude and longitude, any number of destinations can be set, just change index.
 * 
 * EXAMPLE: gps.setDestinations(LATITUDE, LONGITUDE, DESTINATION NUMBER);
 * 
 * It's best practice to use Google Maps and click "What's here" as an example: 40.091524, -75.165870.
 */


int detectionRange = 40; //CENTIMETERS. DETERMINES HOW FAR OBJECT CAN BE DETECTED BEFORE TURNING.


//Set destinations in here!
void SetYourDestinationsHere(){

//Paste your destinations in here. Examples:

    //gps.setDestinations(40.176001, -75.274005, 0); //Goes to Montgomery County Community College.
    gps.setDestinations(0, -1, 0); //Goes to Montgomery County Community College.
  /*
   * gps.setDestinations(42.176001, -72.274005, 1);
   * gps.setDestinations(44.176001, -71.274005, 2);
   * gps.setDestinations(43.176001, -72.274005, 3);
   * gps.setDestinations(41.176001, -74.274005, 4);
   */
  
}



//DO NOT TOUCH BELOW!!!!!


void setup() {
  autoDrive.setup(true);
  autoDrive.detectionRange = detectionRange;
  gps = autoDrive.kalman.roverGPS;
  SetYourDestinationsHere();
  autoDrive.kalman.roverGPS.traverseDestination();
}

void loop() {
  autoDrive.loop();
}
