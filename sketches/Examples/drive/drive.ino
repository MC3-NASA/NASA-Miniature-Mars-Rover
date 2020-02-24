#include <echo.h>
#include <DriveTrain.h>

#include <kalmanFilter.h>
kalmanFilter kalman;

// Lewansoul serial port communication, tx(transmit) goto rx(recieve) on the m4


enum State {
  ACTIVE,
  UNACTIVE,
  FORWARD,
  BACKWARD,
  SUSPENDED
  };

 enum Action {
    NOTHING,
    STOP,
    TURNAROUND,
    moveBack
  };

DriveTrain drive;
State roboState;
Action action;
echo frontDetector;

long elapsedTime;
long currentTime;

int prevTime;

void setup() {
  Serial.begin(9600);
  drive.setup();
  stopRover();
  kalman.setup();

  frontDetector.setup();
  //driveTestRoutine();
  //resetPosition();
  action = NOTHING;
  roboState = ACTIVE;

  currentTime = 0;
  prevTime = 0;
}

void loop() {
  frontDetector.loop();
  kalman.loop();

  Serial.println(frontDetector.distance);

  
  
    
    switch(roboState) {
      case ACTIVE:
        if(canMoveForward)  roboState = FORWARD;
      break;
      case UNACTIVE:
      break;
      
      case FORWARD:
        if(canMoveForward() == 0) {
          action = TURNAROUND;
          roboState = SUSPENDED;
          elapsedTime = 0;
          currentTime = 0;
          prevTime = millis();
          }else if(canMoveForward() == 2) {
            action = moveBack;
            roboState = SUSPENDED;
            elapsedTime = 0;
            currentTime = 0;
            prevTime = millis();
            }


            // here
        moveForward();
        
      break;
      
      case BACKWARD:
      break;
      
      case SUSPENDED:
      
        switch(action) {
          case STOP:
          stopRover();
          action = NOTHING;
          elapsedTime = 0;
          break;

          case moveBack:
          
          movebackward();

          currentTime = millis();
          elapsedTime = currentTime - prevTime;
          Serial.println(elapsedTime);

          if(elapsedTime > 3000) {
            if(canMoveForward() == 2) {
              action = STOP;
              Serial.println("done");
              roboState = SUSPENDED;
              elapsedTime = 0;
              currentTime = 0;
              prevTime = millis();
              stopTurning();
              }else {
            action = TURNAROUND;
            roboState = SUSPENDED;
            elapsedTime = 0;
            currentTime = 0;
            prevTime = millis();
              }
          }
          break;

          case TURNAROUND:
          forwardTurning();
          rotate();

           currentTime = millis();
          elapsedTime = currentTime - prevTime;
          Serial.println(elapsedTime);


          if(elapsedTime > 2000) {
            if(canMoveForward() == 0) {
            action = STOP;
            roboState = SUSPENDED;
            elapsedTime = 0;
            currentTime = 0;
            prevTime = millis();
            stopTurning();
            }else {
            stopTurning();
            roboState = FORWARD;
            }
          }
          break;

          case NOTHING:
            if(canMoveForward() == 1) {
              roboState = FORWARD;
              }else {
                currentTime = millis();
                elapsedTime = currentTime - prevTime;
                Serial.println(elapsedTime);


                if(elapsedTime > 5000)  roboState = UNACTIVE;
                }
          break;
          }
      
      break;
      }


    Serial.println(roboState);

    delay(100);
}

void moveForward() {
  // left side
  drive.spinAt(25, 90);
  drive.spinAt(21, 90);
  drive.spinAt(20, 90);

  // right side
  drive.spinAt(27, -90);
  drive.spinAt(22, -90);
  drive.spinAt(28, -90);
  }


  void movebackward() {
  // left side
  drive.spinAt(25, -40);
  drive.spinAt(21, -40);
  drive.spinAt(20, -40);

  // right side
  drive.spinAt(27, 40);
  drive.spinAt(22, 40);
  drive.spinAt(28, 40);
  }


void stopRover() {
  // left
  drive.spinAt(25, 0);
  drive.spinAt(21, 0);
  drive.spinAt(20, 0);

  // right
  drive.spinAt(27, 0);
  drive.spinAt(22, 0);
  drive.spinAt(28, 0);
  }

// value return is 1 yes 0 no 2 move backwards
int canMoveForward() {
  if(frontDetector.distance < 100 && frontDetector.distance > 0) {
    return 2;
  }else {
    
  if(frontDetector.distance > 100) {
    return 1;
    }
  }
  
  return 0;
  }

void rotate() {
  /* Turn Left */
  
  // front
  drive.moveTo(23, 50);
  drive.moveTo(29, 70);

  // back
  drive.moveTo(24, -50);
  drive.moveTo(26, 50);
  }

void stopTurning() {
  // front
  drive.moveTo(23, 0);
  drive.moveTo(29, 0);

  // back
  drive.moveTo(24, 0);
  drive.moveTo(26, 0);
  }

void forwardTurning() {
  drive.spinAt(25, 60);
  drive.spinAt(21, 60);
  drive.spinAt(20, 60);

  // right side
  drive.spinAt(27, -40);
  drive.spinAt(22, 60);
  drive.spinAt(28, 60);
  }


void resetPosition() {
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

void driveTestRoutine() {
  /* Forward */

  // left
  drive.spinAt(25, 40);
  drive.spinAt(21, 40);
  drive.spinAt(20, 40);

  // right
  drive.spinAt(27, -40);
  drive.spinAt(22, -40);
  drive.spinAt(28, -40);

  delay(5000);
  
  /* Backward */

  // left
  drive.spinAt(25, -40);
  drive.spinAt(21, -40);
  drive.spinAt(20, -40);

  // right
  drive.spinAt(27, 40);
  drive.spinAt(22, 40);
  drive.spinAt(28, 40);

  delay(5000);
}
