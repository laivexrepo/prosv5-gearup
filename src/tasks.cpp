#include "main.h"
#include "globals.h"
#include "portdef.h"
#include "tasks.h"
#include "lift.h"
#include "claw.h"

bool runTaskLift=false;               // controls LIFT if tasks should run.
bool runTaskIntake=false;             // controls INTAKE if tasks should run

void intakeTaskFnc(void* ignore) {
  //the void* is there to provide a way to pass a
  //generic value or structure to the task if needed
  //pros needs this parameter in your function even if you don't use it

  // While intake in this case runs a claw, the sample code is actually
  // runs the "motor" forward or backwards or stopped - it is easily
  // extended to have two motors including running in opposite direction

  float clawAngle = 0;

  std::cout << "Starting Intake Task \n";
  // run the main task loop ONLY in autonomous mode
  if(runTaskIntake) { std::cout << " IntakeTaskFnc: runTask is TRUE \n";} else { std::cout << "runTask is FALSE \n";}
  while(true) {
    while (pros::competition::is_autonomous() || runTaskIntake) {
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
      pros::delay(20);      // DO NOT starve the task!!
    }
    pros::delay(20);        // DO NOT starve the task!!
  }
}


void liftTaskFnc(void* ignore) {
  //the void* is there to provide a way to pass a
  //generic value or structure to the task if needed
  //pros needs this parameter in your function even if you don't use it

  std::cout << "Starting lift Task \n";
  // We assume we have a global variable which will tell us what angle we
  // we want to travel to, based of the 'zero' point.
  // liftMoveAngle  -- angle to move arm at in liftTask
  // liftMoveSpeed  -- speed to move lift at in liftTask
  // liftLastMoveAngle  -- last angle we requested to be moved to

  if(DEBUG) {std::cout << "Lift Move For Angle: " << liftMoveAngle << " Previous Angle: " << liftLastMoveAngle << " \n"; }

  if(runTaskLift) { std::cout << "LiftTAskFnc: runTask is TRUE \n";} else { std::cout << "runTask is FALSE \n";}
  // run the main task loop ONLY in autonomous mode
  while(true) {
    while (pros::competition::is_autonomous() || runTaskLift) {
      if ( liftMoveAngle != liftLastMoveAngle) {
        // We only move the lift IF we have a change in angle request
        liftMoveForAngle(liftMoveAngle, liftMoveSpeed );
        liftLastMoveAngle = liftMoveAngle;
      }
      pros::delay(20);            // DO NOT starve the task!!
    }
    pros::delay(20);              // DO NOT starve the task!!
  }
}
