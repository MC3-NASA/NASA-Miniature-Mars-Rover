#include <kalmanFilter.h>
#include <DriveTrain.h>

bool Turning = true;
kalmanFilter kalman;
DriveTrain drive;
void setup() {
  // put your setup code here, to run once:
  drive.setup();
  kalman.setup();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  drive.forward(0);
    while(Turning){
              delay(10);
        kalman.loop();
      Serial.println("turning");
      if(drive.zeroRadiusTurn(90, kalman.orient)){

        Turning = false;

      }
    }
}
