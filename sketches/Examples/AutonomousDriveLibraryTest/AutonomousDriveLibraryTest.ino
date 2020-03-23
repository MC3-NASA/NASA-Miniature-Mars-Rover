/*
 * Created by Noah Williams on 3/21/2020.
 * 
 * Purpose: Makes Rover drive to a manual destination. Set the latitude and longitude, any number of destinations can be set, just change index.
 * 
 * EXAMPLE: autoDrive.kalman.roverGPS.setDestinations(LATITUDE, LONGITUDE, DESTINATION NUMBER);
 * 
 * It's best practice to use Google Maps and click "What's here" as an example: 40.091524, -75.165870.
 */

#include <AutonomousDrive.h>
AutonomousDrive autoDrive;
void setup() {
  autoDrive.setup();

    // Place destinations here!!!
  autoDrive.kalman.roverGPS.setDestinations(40.176001, -75.274005, 0); //Goes to Montgomery County Community College.
  /*
   * autoDrive.kalman.roverGPS.setDestinations(42.176001, -72.274005, 1);
   * autoDrive.kalman.roverGPS.setDestinations(44.176001, -71.274005, 2);
   * autoDrive.kalman.roverGPS.setDestinations(43.176001, -72.274005, 3);
   * autoDrive.kalman.roverGPS.setDestinations(41.176001, -74.274005, 4);
   */
}

void loop() {
  autoDrive.loop();
}
