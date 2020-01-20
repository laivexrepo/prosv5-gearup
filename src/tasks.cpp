#include "main.h"
#include "globals.h"
#include "portdef.h"
#include "tasks.h"
#include "lift.h"

void intakeTaskFnc(void* ignore) {
  //the void* is there to provide a way to pass a
  //generic value or structure to the task if needed
  //pros needs this parameter in your function even if you don't use it
  //pros::Controller master(pros::E_CONTROLLER_MASTER);
  //pros::Motor left_wheel (LEFT_MOTOR_PORT);
  //pros::Motor right_wheel (RIGHT_MOTOR_PORT);

  std::cout << "Starting Intake Task \n";
  while(true) {
    switch(intakeState){
      default:
      case 0:
         clawMotor1.move_velocity(0);
      break;

      case 1:
         clawMotor1.move_velocity(50);
      break;

      case 2:
        clawMotor1.move_velocity(-50);
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
