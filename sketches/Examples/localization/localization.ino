#define BNO055_SAMPLERATE_DELAY_MS (100)
#include <kalmanFilter.h>
#include <coroutine.h>
kalmanFilter kalman;
coroutine serialCoroutine;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  kalman.setup();
  serialCoroutine.setup(500);
  calibrate();
}

void loop() {
  // put your main code here, to run repeatedly:
  kalman.loop();
  serialCoroutine.loop();
  if(serialCoroutine.readyState){
     kalman.serialize();
     serialCoroutine.reset(); 
  }
    if(kalman.destinationReached()){
    digitalWrite(6, HIGH);
    delay(100);
    digitalWrite(6, LOW);
    delay(100);
  }else{
    digitalWrite(6, HIGH);
  }
  if(abs(kalman.orient.heading - kalman.roverGPS.bearing) < 20){
    digitalWrite(7, HIGH);
  }else{
    digitalWrite(7, LOW);
  }
}

void calibrate(){
  uint8_t system, gyro, accel, mag = 0;
  while (system != 3)
  {
    kalman.orient.bno.getCalibration(&system, &gyro, &accel, &mag);
    Serial.print("CALIBRATION: Sys=");
    Serial.print(system, DEC);
    Serial.print(" Gyro=");
    Serial.print(gyro, DEC);
    Serial.print(" Accel=");
    Serial.print(accel, DEC);
    Serial.print(" Mag=");
    Serial.println(mag, DEC);
    delay(100);
  }
  Serial.println(""); Serial.println("Calibrated");
  delay(2000);
}
