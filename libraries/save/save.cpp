#include "save.h"

void save::setup() {
	// Open serial communications and wait for port to open:
	while (!Serial) {
		; // wait for serial port to connect. Needed for native USB port only
	}


	Serial.print("Initializing SD card...");

	if (!SD.begin(SDCARD_SS_PIN)) {
		Serial.println("initialization failed!");
		while (1);
	}
	Serial.println("initialization done.");
}

void save :: writeFile(char *str) {
	// open the file. note that only one file can be open at a time,
	// so you have to close this one before opening another.
	file = SD.open(str, FILE_WRITE);
	Serial.println(str);

	// if the file opened okay, write to it:
	if (file) {

	}
	else {
		// if the file didn't open, print an error:
		Serial.println("error opening file");
	}


}

void save::saveToFileln(char *str) {
	file.println(str);

}

void save::saveToFile(char *str) {
	file.print(str);

}

void save::saveToFileln(float str) {
	file.println(str);

}

void save::saveToFile(float str) {
	file.print(str);

}

void save :: closeFile() {
	file.close();
	Serial.println("File closed");

}

void save :: removeFile(char *str) {
	SD.remove(str);

}
void save :: openFile(char *str) {
	// re-open the file for reading:
	file = SD.open(str);
	if (file) {

		// read from the file until there's nothing else in it:
		while (file.available()) {
			Serial.write(file.read());
		}
		// close the file:
		file.close();
	}
	else {
		// if the file didn't open, print an error:
		Serial.println("error opening file");
	}
}

void save :: loadMap(char *str, char roverMap[64][64], float*lat, float*lon, int*zoom) {
  // re-open the file for reading:
  String mapString = "";
  file = SD.open(str);
  if (file) {
    // read from the file until there's nothing else in it:
    while (file.available()) {
      mapString += (char)file.read();
    }
    // close the file:
    file.close();
  }
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening file");
  }

  //String processing begins here...
  int indexS = 0;
  indexS=processString(mapString, lat, indexS);
  indexS=processString(mapString, lon, indexS);
  String buffer = "";
  while(mapString[indexS] != '{'){
	  buffer += mapString[indexS];
	  indexS++;
  }
  *zoom = buffer.toInt();
  indexS++;
  for(int i = 0; i < 64; i++){
	  for(int j = 0; j < 64; j++){
		  roverMap[i][j] = mapString[indexS+j+(i*64)];
	  }
  }

}

int save :: processString(String procString, float *value, int indexS){
  String buffer = "";
  while(procString[indexS] != ',' && procString[indexS] != '\0'){
	  buffer += procString[indexS];
	  indexS++;
  }
  *value = buffer.toFloat();
  indexS++;
  return indexS;
}
void save :: loop() {

}