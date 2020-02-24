#include <RoverGPS.h>
/*
 * 
 * Test website: https://www.movable-type.co.uk/scripts/latlong.html
 */
RoverGPS gps;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  gps.setup();
}

void loop() {
  double latStart = 40.081895;
  double lonStart = -75.163826;
  // put your main code here, to run repeatedly:
  //gps.loop();
    //X is latitude.
  //Y is longitude.

  //Latitude is East/West.
  //Longitude is North/South.
  
  //40.081822, -75.163859 BEARING MUST BE: 199 | DISTANCE 8.589 meters.
  //40.081863, -75.163727 BEARING MUST BE: 112
  //40.081986, -75.163781 BEARING MUST BE: 20  | DISTANCE 10.822 meters
  //40.081983, -75.163893 BEARING MUST BE: 329
  double latDest = 40.081986;
  double lonDest = -75.163781;
  gps.calculateBearing(latStart, lonStart,
                latDest, lonDest );

  gps.calculateDistance(latStart, lonStart,
                latDest, lonDest );
  Serial.print(" BEARING | DISTANCE: ");
  String value = (String) " " + String(gps.bearing, 8) + " | " + String(gps.distance, 15);
  Serial.println(value);
  delay(100);
  Serial.println(String(haversine(latStart, lonStart,
               latDest, lonDest), 15));
  delay(2000);
}

double haversine(double lat1, double lon1, 
                        double lat2, double lon2) 
    { 
        // distance between latitudes 
        // and longitudes 
        double dLat = (lat2 - lat1) * 
                      M_PI / 180.0; 
        double dLon = (lon2 - lon1) *  
                      M_PI / 180.0; 
  
        // convert to radians 
        lat1 = (lat1) * M_PI / 180.0; 
        lat2 = (lat2) * M_PI / 180.0; 
  
        // apply formulae 
        double a = pow(sin(dLat / 2), 2) +  
                   pow(sin(dLon / 2), 2) *  
                   cos(lat1) * cos(lat2); 
        double rad = 6371; 
        double c = 2 * asin(sqrt(a)); 
        return rad * c; 
    } 
