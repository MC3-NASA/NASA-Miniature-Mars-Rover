#include <DriveTrain.h>
#include <kalmanFilter.h>

kalmanFilter kalman;

enum State {
  ACTIVE,
  UNACTIVE,
  TURN,
  MOVE,
};

State machine;
DriveTrain drive;

void setup() {
  Serial.begin(9600);
  drive.setup();
  reset();
  calibration();

  // BN055 Noah
  kalman.setup();

  reset();
  machine = ACTIVE;
}

void loop() {
  kalman.loop();
  Serial.println("HI");
  double tolerance = kalman.tolerance;//Let's play with different values.
  double heading = kalman.orient.heading; //Kalman filters it for you.
  double bearing = kalman.roverGPS.bearing; //But you can just call roverGPS (no filter).

  Serial.print("differenc: ");
  Serial.println(abs(heading-bearing));
  /*
  switch(machine) {
  case ACTIVE:
  /*
  if (abs(heading-bearing) < tolerance){
    machine = UNACTIVE;}
  else{
    machine = TURN;
  }
  break;
  case UNACTIVE:
  break;
  case TURN:
  /*
  turn(40);
  if (abs(heading-bearing) < tolerance){
    machine = UNACTIVE;}
  else{
    machine = TURN;
  }
  break;
  case MOVE:
  break;  
  }
  */
  delay(100);
}

void calibration() {

  // Front
  drive.moveTo(23, 50);
  drive.moveTo(29, 70);

  // Back
  drive.moveTo(24, -50);
  drive.moveTo(26, 50);

  // left side
  drive.spinAt(25, 40);
  drive.spinAt(21, 40);
  drive.spinAt(20, 40);

  // right side
  drive.spinAt(27, -40);
  drive.spinAt(22, -40);
  drive.spinAt(28, -40);

  delay(1000);
}

void turn(int speed) {
  
  // Front
  drive.moveTo(23, 50);
  drive.moveTo(29, 70);

  // Back
  drive.moveTo(24, -speed);
  drive.moveTo(26, speed);
}

void reset() {
  /* Turn right */
  // front
  drive.moveTo(23, 0);
  drive.moveTo(29, 0);

  // back
  drive.moveTo(24, 0);
  drive.moveTo(26, 0);

  delay(1000);

  /* Turn Left */
  
  // front
  drive.moveTo(23, 0);
  drive.moveTo(29, 0);

  // back
  drive.moveTo(24, 0);
  drive.moveTo(26, 0);

  delay(1000);

  // left
  drive.spinAt(25, 0);
  drive.spinAt(21, 0);
  drive.spinAt(20, 0);

  // right
  drive.spinAt(27, 0);
  drive.spinAt(22, 0);
  drive.spinAt(28, 0);
}
