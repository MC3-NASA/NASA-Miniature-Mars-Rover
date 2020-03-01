/*
 * Created by Noah Williams 2/29/2020.
 * Purpose: To test waypoint navigation.
 */
#include <RoverGPS.h>
#include <kalmanFilter.h> //Gets IMU device for the rover
#include <coroutine.h>
RoverGPS GPS; //A common library for IMU devices.
coroutine printCoroutine;
kalmanFilter kalman; //A common library for IMU devices.
void setup() {
  // put your setup code here, to run once:
  GPS.setup();
  printCoroutine.setup(2000);
  GPS.setDestinations(40.176001, -75.274005, 0); //First tree south tree.
  GPS.setDestinations(40.176103, -75.273777, 1); //Second east tree.
  GPS.setDestinations(40.176275, -75.274011, 2); //Third starting point.
}

void loop() {
  // put your main code here, to run repeatedly:
  GPS.loop();
  if(printCoroutine.readyState){
      GPS.serialize();
      printCoroutine.reset();
      GPS.traverseDestination(); //Every print should traverse this number. Until the last desintation is printed.
  }
  printCoroutine.loop();
  kalman.destinationReached();
}
