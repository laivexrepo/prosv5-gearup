#ifndef IMU_H_
#define IMU_H_

// drivebase IMU based functions
float pivotForAngleWithIMU(float angle, int speed, bool isPivot);  // IMU based turn

int imuInit();
void imuHeadingTest();
void imuAccelerometerTest();

#endif
