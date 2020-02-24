#include <imumaths.h>

imu::Matrix<3> A = imu::Matrix<3>();
imu::Matrix<3> B = imu::Matrix<3>();
void setup() {
  // put your setup code here, to run once:
  imu::Vector<3> a = imu::Vector<3>(1, 2, 3);
  A.vector_to_row(a, 0);
  A.vector_to_row(a*2, 1);
  A.vector_to_row(a*3, 2);

  imu::Vector<3> b = imu::Vector<3>(1, 1, -1);
  
  B.vector_to_row(b, 0);
  B.vector_to_row(b*2, 1);
  B.vector_to_row(b*3, 2);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Matrix A: ");
  printMatrix(A, 3);
  Serial.println("Matrix B: ");
  printMatrix(B, 3);
  Serial.println("Matrix AB: ");
  printMatrix(A*B, 3);
  delay(2000);
}

void testMultiplication(){
  
}

void printMatrix(imu::Matrix<3> a, int s){

  for(int i = 0; i < s; i++){
        Serial.print("[ ");
    for(int j = 0; j < s; j++){
         Serial.print(a.cell(i,j));
         Serial.print(" ");
    }
    Serial.println(" ]");
  }

}
