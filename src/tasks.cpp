#include "main.h"
#include "globals.h"
#include "portdef.h"
#include "tasks.h"
#include "lift.h"
#include "claw.h"

void intakeTaskFnc(void* ignore) {
  //the void* is there to provide a way to pass a
  //generic value or structure to the task if needed
  //pros needs this parameter in your function even if you don't use it

  // While intake in this case runs a claw, the sample code is actually
  // runs the "motor" forward or backwards or stopped - it is easily
  // extended to have two motors including running in opposite direction
  
  float clawAngle = 0;

  std::cout << "Starting Intake Task \n";
  while(true) {
    switch(intakeState){
      default:
      case 0:                    // claw closed / locked
         clawMotor1.move_velocity(0);
         // sample of using a function to open  close a claw
         // clawAngle = clawOpenForAngle(0,35);
      break;

      case 1:                   // open claw
         clawMotor1.move_velocity(50);
         // sample of using a function to open  close a claw
         // clawAngle = clawOpenForAngle(70,35);
      break;

      case 2:                   // close claw
        clawMotor1.move_velocity(-50);
        // sample of using a function to open  close a claw
        // clawAngle = clawOpenForAngle(0,35);
      break;
    }
    pros::delay(20);
  }
}

void liftTaskFnc(void* ignore) {
  //the void* is there to provide a way to pass a
  //generic value or structure to the task if needed
  //pros needs this parameter in your function even if you don't use it
  //pros::Controller master(pros::E_CONTROLLER_MASTER);
  //pros::Motor left_wheel (LEFT_MOTOR_PORT);
  //pros::Motor right_wheel (RIGHT_MOTOR_PORT);

  std::cout << "Starting lift Task \n";
  // We assume we have a global variable which will tell us what angle we
  // we want to travel to, based of the 'zero' point.
  // liftMoveAngle  -- angle to move arm at in liftTask
  // liftMoveSpeed  -- speed to move lift at in liftTask
  // liftLastMoveAngle  -- last angle we requested to be moved to

  if(DEBUG) {std::cout << "Lift Move For Angle: " << liftMoveAngle << " Previous Angle: " << liftLastMoveAngle << " \n"; }
  while(true) {
    if ( liftMoveAngle != liftLastMoveAngle) {
       // We only move the lift IF we have a change in angle request
       liftMoveForAngle(liftMoveAngle, liftMoveSpeed );
       liftLastMoveAngle = liftMoveAngle;
    }
    pros::delay(20);
  }
}
