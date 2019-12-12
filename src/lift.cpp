#include "main.h"
#include "globals.h"
#include "portdef.h"
#include "lift.h"

float maxAngleMovement = 90;       // Maximum angle lift can move to

int liftGearRatio = 7;              // gear ratio between motor and lift arm
                                    // if a 12gear drives a 84gear the ratio is 84/12 = 7
                                    // for direct drive it is 1

void liftControl(int speedUp, int speedDown){
  if (master.get_digital(DIGITAL_R1)) {
     liftMotor.move_velocity(speedUp);
   }
   else if (master.get_digital(DIGITAL_R2)) {
     liftMotor.move_velocity(-speedDown);
   }
   else {
     liftMotor.move_velocity(0);
   }
}

void liftSetLowPreset() {
  // Set zero point of the lift, by doing a tare_position on the lift motors,
  // from which point forward all movement can be calculated by number of
  // degree of rotation upwards/downwards for the arm to move.

  // the tare_position is set when an assigned button on the controller is
  // pressed, it writes to the LCD screen on the brain that it is awaiting
  // the setting of a 0 point.

  // This function should be called in the void competition_initialize() function of
  // main.cpp

  liftMotor.tare_position();
  if(DEBUG) { std::cout << "Lift Encoder: " << liftMotor.get_position() << " \n"; }

  pros::lcd::print(2, "ZERO point for lift set");
  pros::delay(20);
}

float liftMoveForAngle(float angle, int speed ){
  // moves the lift to a given position based on angle provided, it returns the
  // angle of the lift where it has stopped
  if(DEBUG) { std::cout << "Request angle: " << angle << " speed: " << speed << "\n";}
  if(DEBUG) { std::cout << "Gear Ratio: " << liftGearRatio << " New angle: " << angle * liftGearRatio << "\n"; }

  angle = liftGearRatio * angle;            // account for gear Ratio

  // get current angle of the lift.
  float currentAngle = liftMotor.get_position();

  if(angle < 0) {
    if(DEBUG) { std::cout << "Can not move down past ZERO position \n"; }
  } else {
    if(angle > (maxAngleMovement * liftGearRatio) ) {
      // We need to accomedate gear ratio in calculation the true angle is adjusted
      // by liftGearRatio for the actual motor move angle.
      if(DEBUG) { std::cout << "Can not move past maximum angle of:" << maxAngleMovement << " \n"; }
    } else {
      // we can move so lets....
      // calculate the target window
      float minAngle = angle - 5;
      float maxAngle = angle + 5;
      if(DEBUG) { std::cout << "angle: " << angle << " minAngle: " << minAngle << " maxAngle: " << maxAngle << "\n";}
      liftMotor.move_absolute(angle, speed);
      while (!((liftMotor.get_position() < maxAngle) && (liftMotor.get_position() > minAngle))) {
        pros::delay(2);
      }
    }
  }
  if(DEBUG) { std::cout << "Final angle: " << liftMotor.get_position() << " True angle: " << (liftMotor.get_position() / liftGearRatio) << " \n"; }
  return(liftMotor.get_position());
}
