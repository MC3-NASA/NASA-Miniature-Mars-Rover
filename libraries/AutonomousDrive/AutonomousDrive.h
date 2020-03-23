#include <DriveTrain.h> //Deals with spinning the wheels of the rover.
#include <kalmanFilter.h> //Gets IMU device for the rover
#include "coroutine.h"
#include <echo.h>
class AutonomousDrive {
	public:
        kalmanFilter kalman; //A common library for IMU devices.
        DriveTrain drive;
        coroutine driveCoroutine;
        coroutine kalmanCoroutine;
        coroutine avoid;
        coroutine printData;

        enum Rover {
        TRACK,
        BACKUP,
        AVOID,
        SUCCESS,
        };

        Rover machine;
        echo echosensor;

        double tolerance = kalman.tolerance;//Let's play with different values.
        double heading = kalman.orient.heading; //Kalman filters it for you.
        double bearing = kalman.roverGPS.bearing; //But you can just call roverGPS (no filter).
        double difference = abs(heading-bearing);
        int wheelDirection = 0;
        int detectionRange = 50; //Centimeters

        void setup();
        void setup(bool isManual);
        void calibrate();
        void loop();
        void backup();
        void avoidObstacle();
        void followBearing();
        void forwards(int speed);
        void spin(int r, int motors);
        void reset();
        void haltRover();
        void serializeData();
};
