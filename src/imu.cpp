#include "imu.h"
#include "portdef.h"
#include "globals.h"
#include "math.h"
#include "drivebase.h"

// Contains all the IMU based functions

// IMU heading Tester Code - only works in opcontrol() for testing
void imuHeadingTest(){
    bool runIMUtest = true;
    if(DEBUG) { std::cout << "Running IMU Heading Test Routine \n"; }
    while(runIMUtest) {
      if (master.get_digital(DIGITAL_B)) {
        runIMUtest = false;
        if(DEBUG){ std::cout << "Ending IMU Heading Test routine \n"; }
      }
      if(DEBUG) { std::cout << "Current Heading: " << imu_sensor.get_heading() << " \n"; }
      pros::delay(20);
    }
}

// ------ drivebase IMU based functions -----------------------
float pivotForAngleWithIMU(float angle, int speed){
  float finalTurnHeading;               // we will return the final angle we actually
                                        // turned for.

  // We are using P-loop to turn the robot precisely
  float Kp;             // P-loop K tuning variable - how fast are we closing error
  float error;          // error - how far are we still of target
  float targetHeading;  // target heading we are turning towards
  float power;          // power (speed) to run motors at
  float startingHeading;// heading of IMU prior to move

  //int maxDriveRPM = MOTOR_MAX_SPEED;        // global value from drivebase.h

  int maxDriveRPM = speed;                  // can never go faster then the requested speed

  bool turnCW = true;                       // if a right turn for positive angle request (CW),
                                            // it will be false for negative angle (CCW) left turn
                                            // all as seen from the back of the robot.


  // we will use a proprotional speed loop, where as we get closer to the
  // requested angle of turn we slow down the motors. so the potential of
  // overshooting the desired turn is minimized

  //P-loop constant
  Kp = 0.2;

  bool reachedTarget = false;       // helps us end P-loop - becomes true
                                    // when we have reached target Angle

  if(angle > 0) {
    turnCW=true;
    if(DEBUG) { std::cout << "Turning Right CW - postive angle \n";}
  } else {
    turnCW=false;
    if(DEBUG) { std::cout << "Turning Left CCW - negative angle \n";}
  }

  // We need to deal with passing the 360/0 heading so if current heading is 300 degrees
  // and we are asked to turn from thsi position another 90 degree - i.e target would be
  // 300 + 90 = 390 degrees we are actually need to look for heading target - 360 = 30degrees

  if(turnCW) {
     startingHeading = imu_sensor.get_heading();
     if(DEBUG) { std::cout << "Starting Heading for pivot Turn (CW): " << startingHeading << "\n";}
     targetHeading = angle;

     if(DEBUG) { std::cout << "Pivot turn angle requested: " << angle << " Normalized Heading: " << targetHeading << "\n";}
     if(DEBUG) { std::cout << "Starting Power for pivot Turn: " << speed << "\n";}

     power = speed;

     while(imu_sensor.get_heading() < targetHeading && !reachedTarget){
       // P-loop
       error = targetHeading - imu_sensor.get_heading();
       power = abs(power * error * Kp);             // we want to keep power ABS
       if(DEBUG) { std::cout << "Calculated Power: " << power << " \n"; }
       if(abs(power) > maxDriveRPM) {
         power = maxDriveRPM;
       }
       if(DEBUG) { std::cout << "Error: " << error << " Power: " << power << " Current Heading: " << imu_sensor.get_heading() << "\n"; }
       // now turn the robot -- we either turn clockwise
       left_wheel.move_velocity(power);
       right_wheel.move_velocity(-power);

       pros::delay(20);                 // keep CPU from being hogged
     }
  } else {
    startingHeading = imu_sensor.get_heading();
    if(DEBUG) { std::cout << "Starting Heading for pivot Turn (CCW): " << startingHeading << "\n";}

    targetHeading = startingHeading + angle;
    if(targetHeading < 0) { targetHeading = 360 + targetHeading; }

    if(DEBUG) { std::cout << "Pivot turn angle requested: " << angle << " Normalized Heading: " << targetHeading << "\n";}
    if(DEBUG) { std::cout << "Starting Power for pivot Turn: " << speed << "\n";}

    power = speed;

    reachedTarget = false;

    while(imu_sensor.get_heading() > targetHeading && !reachedTarget){
      // P-loop
      error = targetHeading - imu_sensor.get_heading();
      power = abs(power * error * Kp);             // we want to keep power ABS
      if(DEBUG) { std::cout << "Calculated Power: " << power << " \n"; }
      if(abs(power) > maxDriveRPM) {
        power = maxDriveRPM;
      }
      if(DEBUG) { std::cout << "Error: " << error << " Power: " << power << " Current Heading: " << imu_sensor.get_heading() ;
         std::cout << " targetHeading: " << targetHeading << "\n";
      }

      // now turn the robot -- we turn counter clockwise
      left_wheel.move_velocity(-power);
      right_wheel.move_velocity(power);

      pros::delay(20);                 // keep CPU from being hogged
    }
  }
  finalTurnHeading=imu_sensor.get_heading();
  if(DEBUG) { std::cout << "Reached target heading: " << finalTurnHeading << "\n";}
  // stop movement
  left_wheel.move_velocity(0);
  right_wheel.move_velocity(0);

  return(finalTurnHeading);
}
