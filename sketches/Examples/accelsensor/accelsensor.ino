#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <save.h>
#include <SPI.h>
#include <SD.h>
#include <matrix.h>

#include <save.h>
#include <button.h>
save save;
button button;
/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);

matrix<double, (int)3, (int)3> matrix2;
matrix<double, (int)3, (int)3> matrix3;
matrix<double, (int)3,(int)3> m;

void displaySensorDetails(void)
{
  sensor_t sensor;
  accel.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  sensor.resolution = 4;
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void setup(void)
{
  
  double a[3][3] = {1,0,0,
                    0,1,0,
                    0,0,1};
                    
  double b[3][3] = {1,2,3,
                    4,5,6,
                    7,8,9};
                    
  matrix3.setup(b);
  matrix2 = matrix<double, (int)3,(int)3>(a);
  
  m = matrix<double, (int)3,(int)3>(a);
  
  m = matrix2*matrix3;
  save.setup();
  button.setup(2);
  save.writeFile("acel.txt");
#ifndef ESP8266
  while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
#endif
  Serial.begin(9600);
  Serial.println("Accelerometer Test"); Serial.println("");



  /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }

  /* Display some basic information on this sensor */
  displaySensorDetails();

}

void loop(void)
{
  /* Get a new sensor event */
  sensors_event_t event;
  accel.getEvent(&event);

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");

  /* Note: You can also get the raw (non unified values) for */
  /* the last data sample as follows. The .getEvent call populates */
  /* the raw values used below. */
  Serial.print("X Raw: "); Serial.print(accel.raw.x); Serial.print("  ");
  Serial.print("Y Raw: "); Serial.print(accel.raw.y); Serial.print("  ");
  Serial.print("Z Raw: "); Serial.print(accel.raw.z); Serial.println("");
  //matrix2.serialize(matrix2.mat);

/*
 * Saves data to file.
 */
  save.saveToFile("X: "); save.saveToFile(event.acceleration.x); save.saveToFile("  ");
  save.saveToFile("Y: "); save.saveToFile(event.acceleration.y); save.saveToFile("  ");
  save.saveToFile("Z: "); save.saveToFile(event.acceleration.z); save.saveToFile("  ");save.saveToFileln("m/s^2 ");
  save.loop();
  //m.serialize();
  matrix2.serialize();
  /* Delay before the next sample */
  delay(500);

  if(button.checkButton()){
    save.closeFile();
    save.openFile("acel.txt");
  }
}
