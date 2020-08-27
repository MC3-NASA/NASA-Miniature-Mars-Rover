#include <kalmanFilter.h> //Gets IMU device for the rover
#include "coroutine.h"
#include <echo.h>
#include <save.h>
kalmanFilter kalman;
save save;
//Sketch saves calibration values to SD card.
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  kalman.setup();
  calibrate();
  save.setup();
  SD.remove("CALI.txt");
  save.writeFile("CALI.txt");
}

void loop() {
  // put your main code here, to run repeatedly:
  adafruit_bno055_offsets_t caliData;
  kalman.orient.bno.getSensorOffsets(caliData);
  displaySensorOffsets(caliData);
  delay(1000);
  Serial.println("");
  save.saveCalibrationData(caliData);
  save.write_STRUCT("CALL", caliData);
}

void calibrate(){
  uint8_t system, gyro, accel, mag = 0;
  while (system != 3)
  {
    kalman.orient.bno.getCalibration(&system, &gyro, &accel, &mag);
    Serial.print("CALIBRATING PLEASE SPIN AROUND!   sys=");
    Serial.print(system, DEC);
    Serial.print(" Gyro=");
    Serial.print(gyro, DEC);
    Serial.print(" Accel=");
    Serial.print(accel, DEC);
    Serial.print(" Mag=");
    Serial.println(mag, DEC);
    delay(100);
  }

  Serial.println(""); Serial.println("Calibration Done. Please place Rover down.");
  delay(10000);
}

void displaySensorOffsets(const adafruit_bno055_offsets_t &calibData)
{
    Serial.print("Accelerometer: ");
    Serial.print(calibData.accel_offset_x); Serial.print(",");
    Serial.print(calibData.accel_offset_y); Serial.print(",");
    Serial.print(calibData.accel_offset_z); Serial.print(",");

    Serial.print("\nGyro: ");
    Serial.print(calibData.gyro_offset_x); Serial.print(",");
    Serial.print(calibData.gyro_offset_y); Serial.print(",");
    Serial.print(calibData.gyro_offset_z); Serial.print(",");

    Serial.print("\nMag: ");
    Serial.print(calibData.mag_offset_x); Serial.print(",");
    Serial.print(calibData.mag_offset_y); Serial.print(",");
    Serial.print(calibData.mag_offset_z); Serial.print(",");

    Serial.print("\nAccel Radius: ");
    Serial.print(calibData.accel_radius);

    Serial.print("\nMag Radius: ");
    Serial.print(calibData.mag_radius);
}
