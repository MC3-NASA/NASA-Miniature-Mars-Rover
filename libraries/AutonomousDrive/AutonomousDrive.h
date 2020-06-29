#pragma once
#include <DriveTrain.h> //Deals with spinning the wheels of the rover.
#include <kalmanFilter.h> //Gets IMU device for the rover
#include "coroutine.h"
#include <echo.h>
#include <save.h>
class AutonomousDrive {
	public:
        kalmanFilter kalman; //A common library for IMU devices.
        DriveTrain drive;
        coroutine driveCoroutine;
        coroutine kalmanCoroutine;
        coroutine avoid;
        coroutine printData;
        coroutine saveData;

        enum Rover {
        TRACK,
        BACKUP,
        AVOID,
        SUCCESS,
        TURNINPLACE,
        };

        Rover machine;
        echo echosensorRight;
        echo echosensorLeft;
        save saveSD;
        int LeftTrigPin = 7;
        int LeftRecievePin = 8;
        int RightTrigPin = 9;
        int RightRecievePin = 10;

        double tolerance = kalman.tolerance;//Let's play with different values.
        double heading = kalman.orient.heading; //Kalman filters it for you.
        double bearing = kalman.roverGPS.bearing; //But you can just call roverGPS (no filter).
        double difference = abs(heading-bearing);
        int wheelDirection = 0;
        int detectionRange = 0; //Centimeters
        bool objectDetection = true;
        bool SDRecord = true;
        bool SerializeDataEnabled = true;
        double backupMeters = 1; //How much to backup calculated in meters.

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
        bool driveToMeter(float meter);
        void turnInPlace();
};
