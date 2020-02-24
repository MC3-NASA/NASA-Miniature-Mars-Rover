#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <SPI.h>
#include <SD.h>
#include <matrix.h>

#include <save.h>
#include <button.h>
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);

matrix<double, (int)3, (int)3> matrix2;
matrix<double, (int)3, (int)3> matrix3;
matrix<double, (int)3,(int)3> m;


void setup(void)
{
  
  double a[3][3] = {1,0,0,
                    1,0,0,
                    1,0,0};
                    
  double b[3][3] = {1,2,3,
                    4,5,6,
                    7,8,9};
                    
  matrix3.setup(b);
  matrix2 = matrix<double, (int)3,(int)3>(a);
  
  m = matrix<double, (int)3,(int)3>(a);
  
  m = matrix2*matrix3;
  Serial.begin(9600);

  /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }

}

void loop(void)
{
  sensors_event_t event;
  accel.getEvent(&event);
  m.serialize();
  delay(500);
}
