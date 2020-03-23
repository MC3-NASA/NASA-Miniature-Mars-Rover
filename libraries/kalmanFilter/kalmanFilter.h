#include "orientation.h"
#include "RoverGPS.h"
#include "Arduino.h"

class kalmanFilter {
	public:
		
		imu::Vector<3> averageCalibration = imu::Vector<3>(0, 0, 0);

		imu::Vector<2> location = imu::Vector<2>(0, 0); //The final location, when all is filtered and finished. Latitiude = x, Longitude = y.
		double tolerance = 3.0; //Determines how precise acceleration calculation will be.
		orientation orient; //Gets the IMU.
		RoverGPS roverGPS; //Gets the GPS.

		void loop();
		void setup();
		void debug();
		void calibrate();
		void serialize();
		void predict();
		bool destinationReached();
		float destinationRadius = 6.0f; //How close until destination is conisdered reached.
		double destinations[10];




		//Disabled Variables for future testing.
		/*
		//Kalman Filter variables.
		//These variables represent various states of the kalman filter. Some may be commented out as the use case
		//developed into various functions rather than a single matrix. Here they are listed for the programmer.
		imu::Matrix<2> Q = imu::Matrix<2>(); //Covariance Matrix accelerometer.
		imu::Matrix<2> R = imu::Matrix<2>(); //Covariance matrix GPS. All GPS has 3 meters of error.


		imu::Matrix<2> Kx = imu::Matrix<2>(); //Fused data
		imu::Matrix<2> Hx = imu::Matrix<2>(); //Transformation Matrix
		imu::Matrix<2> Px = imu::Matrix<2>(); //New estimate.
		imu::Vector<2> Zx = imu::Vector<2>(); //Measurement of the slower more stable device, in this case GPS. P and V
		imu::Matrix<2> Ax = imu::Matrix<2>(); //State transition matrix. Position and Velocity
		imu::Matrix<2> Bx = imu::Matrix<2>(); //Control matrix. Equations that change overtime.
		double Ux = 0; //The measurement to be corrected. In this case, the accelerometer.

		
		imu::Matrix<2> Ky = imu::Matrix<2>(); //Fused data
		imu::Matrix<2> Hy = imu::Matrix<2>(); //Transformation Matrix
		imu::Matrix<2> Py = imu::Matrix<2>(); //New estimate.
		imu::Vector<2> Zy = imu::Vector<2>(); //Measurement of the slower more stable device, in this case GPS. P and V
		imu::Matrix<2> Ay = imu::Matrix<2>(); //State transition matrix. Position and Velocity
		imu::Matrix<2> By = imu::Matrix<2>(); //Control matrix. Equations that change overtime.
		double Uy = 0; //The measurement to be corrected. In this case, the accelerometer.
		*/


};
