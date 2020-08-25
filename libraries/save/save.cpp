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
  while(procString[indexS] != ',' && procString[indexS] != ']' &&  procString[indexS] != '[' && procString[indexS] != '\0'){
	  buffer += procString[indexS];
	  indexS++;

  }
  *value = buffer.toFloat();
  indexS++;
  return indexS;
}
void save :: loop() {

}

void save :: loadCalibrationData(Adafruit_BNO055 &bno){
	String cali = ""; //The calibration data stored as a single string.
	adafruit_bno055_offsets_t calibrationData; //Calibration data to be stored within BNO055.
	file = SD.open("CALI.TXT");

		if (file) {
			// read from the file until there's nothing else in it:
			while (file.available()) {
				cali += (char)file.read();
			}
			// close the file:
    	file.close();

		//After reading through the entire text begin to parse data.
		Serial.println(cali); //Prints information for debugging.
		
		//Process accelerometer data.
		int index = 2;
		float value = -1;
		index = processString(cali, &value, index);
		calibrationData.accel_offset_x = value;
		index = processString(cali, &value, index);
		calibrationData.accel_offset_y = value;
		index = processString(cali, &value, index);
		calibrationData.accel_offset_z = value;

		Serial.println(" ");
		//Process gyro data.
		index += 3;
		index = processString(cali, &value, index);
		calibrationData.gyro_offset_x = (int)value;
		Serial.println(value);
		index = processString(cali, &value, index);
		calibrationData.gyro_offset_y = 6496;
		Serial.println(value);
		index = processString(cali, &value, index);
		calibrationData.gyro_offset_z = (int)value;
		Serial.println(value);


		Serial.println(" ");
		//Process magnometer data.
		index += 4;
		index = processString(cali, &value, index);
		calibrationData.mag_offset_x = (int)value;
		Serial.println(value);
		index = processString(cali, &value, index);
		calibrationData.mag_offset_y = (int)value;
		Serial.println(value);
		index = processString(cali, &value, index);
		calibrationData.mag_offset_z = (int)value;
		Serial.println(value);

		//Process radius data.
		Serial.println(" ");
		index += 4;
		index = processString(cali, &value, index);
		calibrationData.accel_radius = (int)value;
		Serial.println(value);
		index += 4;
		index = processString(cali, &value, index);
		calibrationData.mag_radius = (int)value;
		Serial.println(value);

		bno.setSensorOffsets(calibrationData);
		}else{
			// if the file didn't open, print an error:
    		Serial.println("No file, calibrating manually.");
		}


		
}

void save :: saveCalibrationData(adafruit_bno055_offsets_t &calibData)
{
	//Get Acceleration Offsets.
	String str = "a[" + String(calibData.accel_offset_x) + "," + String(calibData.accel_offset_y) + "," + String(calibData.accel_offset_z)+ "]"; 
	int str_len = str.length() + 1;
	char stringarray[str_len];
	str.toCharArray(stringarray, str_len);
	saveToFileln(stringarray);
	
	//Get Gyro Offsets.
	str = "g[" + String(calibData.gyro_offset_x) + "," + String(calibData.gyro_offset_y) + "," + String(calibData.gyro_offset_z)+ "]"; 
	str_len = str.length() + 1;
	char stringarray2[str_len];
	str.toCharArray(stringarray2, str_len);
	saveToFileln(stringarray2);

	//Get Magnometer Offsets.
	str = "m[" + String(calibData.mag_offset_x) + "," + String(calibData.mag_offset_y) + "," + String(calibData.mag_offset_z)+ "]"; 
	str_len = str.length() + 1;
	char stringarray3[str_len];
	str.toCharArray(stringarray3, str_len);
	saveToFileln(stringarray3);
	
	//Get Accel Radius Offsets.
	str = "C[" + String(calibData.accel_radius) + "]"; 
	str_len = str.length() + 1;
	char stringarray4[str_len];
	str.toCharArray(stringarray4, str_len);
	saveToFileln(stringarray4);

	//Get Mag Radius Offsets.
	str = "R[" + String(calibData.mag_radius) + "]"; 
	str_len = str.length() + 1;
	char stringarray5[str_len];
	str.toCharArray(stringarray5, str_len);
	saveToFileln(stringarray5);
	
	closeFile();
}