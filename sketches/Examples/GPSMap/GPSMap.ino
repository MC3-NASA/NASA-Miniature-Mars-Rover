/*
 * Created by Noah Williams on 2/23/2020.
 * Purpose: Loads a map stored within SD card, and uses that map for navigation. This shows such an example.
 */
#include<Map.h>
Map internalMap;
void setup() {
  // put your setup code here, to run once:
  internalMap.setup();
  internalMap.loadMap();
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(internalMap.zoom);
  //Serial.println(String(internalMap.latitude, 8));
  //Serial.println(String(internalMap.longitude, 8));
  //internalMap.printMap();
  imu::Vector<2> vec = imu::Vector<2>(-1,1);
  vec = internalMap.getRealPosition(0,0,vec);
  Serial.println(String(vec.x(),15));
  Serial.println(String(vec.y(),15));
  delay(1000);
}
