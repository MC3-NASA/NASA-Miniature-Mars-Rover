
#include <save.h>
save save;
char roverMap[64][64];
float lat = 0;
float lon = 0;
int zoom = 0;
String mapString;
void setup() {
  // put your setup code here, to run once:
  save.setup();

}

void loop() {
  // put your main code here, to run repeatedly:
  //
  //Serial.print(mapString);
  save.loadMap("MAPROVER.txt", roverMap, &lat, &lon, &zoom);
  Serial.println(String(lat, 8));
  Serial.println(String(lon, 8));
  Serial.println(zoom);
  printMap();
  Serial.println(" ");
  getMap("MAPROVER.txt");
  Serial.println(mapString);
  delay(2000);

}

void printMap(){
  for(int i = 0; i < 64; i++){
    for(int j = 0; j < 64; j++){
      Serial.print(roverMap[i][j]);
    }
  }
}

void getMap(char* str){
  // re-open the file for reading:
  save.file = SD.open(str);
  if (save.file) {
      mapString = "";
    // read from the file until there's nothing else in it:
    while (save.file.available()) {
      mapString += (char)save.file.read();
    }
    // close the file:
    save.file.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening file");
  }
  
}
