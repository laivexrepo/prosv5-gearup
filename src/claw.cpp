#include "main.h"
#include "globals.h"
#include "portdef.h"
#include "claw.h"

float maxAngleClawOpen = 85;        // Maximum angle for the claw to move to
                                    // when it is at it's most open position

int clawGearRatio = 3;              // gear ratio between motor and claw arm gear
                                    // if a 12gear drives a 36gear the ratio is 36/12 = 3
                                    // for direct drive it is 1

void clawControl(){
  // you could make clawCOntrol also take an input varibale for speed,
  // perhaps an speedOpen and a speedClose
  if (master.get_digital(DIGITAL_L1)) {
     clawMotor1.move_velocity(50);
     // If two intake (claw) motors uncomment this - motor spins by default
     // opposite direction of clawMotor1
     //clawMotor2.move_velocity(50);
   }
   else if (master.get_digital(DIGITAL_L2)) {
     clawMotor1.move_velocity(-50);
     // If two intake (claw) motors uncomment this - motor spins by default
     // opposite direction of clawMotor1
     //clawMotor2.move_velocity(-50);
   }
   else {
     clawMotor1.move_velocity(0);
     // If two intake (claw) motors uncomment this - motor spins by default
     // opposite direction of clawMotor1
     //clawMotor2.move_velocity(0);
   }
}

void clawSetLowPreset() {
  // Set zero point of the claw, by doing a tare_position on the claw motors,
  // from which point forward all movement can be calculated by number of
  // degree of rotation upwards/downwards -- inwards/outwards for the claw.

  // the tare_position is set when an assigned button on the controller is
  // pressed, it writes to the LCD screen on the brain that it is awaiting
  // the setting of a 0 point.

  // This function should be called in the void competition_initialize() function of
  // main.cpp

  clawMotor1.tare_position();
  if(DEBUG) { std::cout << "Claw Encoder: " << clawMotor1.get_position() << " \n"; }

  pros::lcd::print(2, "ZERO point for claw set");
  pros::delay(20);
}

float clawOpenForAngle(float angle, int speed ){
  // moves the claw to a given open position based on angle provided,
  // it returns the angle the claw has moved to when it has stopped moving
  if(DEBUG) { std::cout << "Request angle: " << angle << " speed: " << speed << "\n";}
  if(DEBUG) { std::cout << "Gear Ratio: " << clawGearRatio << " New angle: " << angle * clawGearRatio << "\n"; }

  angle = clawGearRatio * angle;            // account for gear Ratio

  // get current angle of the lift.
  float currentAngle = clawMotor1.get_position();

  if(angle < 0) {
    if(DEBUG) { std::cout << "Can not move down past ZERO position \n"; }
  } else {
    if(angle > (maxAngleClawOpen * clawGearRatio) ) {
      // We need to accomedate gear ratio in calculation the true angle is adjusted
      // by liftGearRatio for the actual motor move angle.
      if(DEBUG) { std::cout << "Can not move past maximum angle of:" << maxAngleClawOpen << " \n"; }
    } else {
      // we can move so lets....
      // calculate the target window
      float minAngle = angle - 5;
      float maxAngle = angle + 5;
      if(DEBUG) { std::cout << "angle: " << angle << " minAngle: " << minAngle << " maxAngle: " << maxAngle << "\n";}
      clawMotor1.move_absolute(angle, speed);
      while (!((clawMotor1.get_position() < maxAngle) && (clawMotor1.get_position() > minAngle))) {
        pros::delay(2);
      }
      clawMotor1.move_velocity(0);     // make sure we stop
    }
  }
  if(DEBUG) { std::cout << "Final angle: " << clawMotor1.get_position() << " True angle: " << (clawMotor1.get_position() / clawGearRatio) << " \n"; }
  return(clawMotor1.get_position());
}
