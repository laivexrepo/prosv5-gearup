#include "main.h"
#include "globals.h"
#include "portdef.h"
#include "tasks.h"
#include "lift.h"
#include "claw.h"

bool runTaskLift=false;               // controls LIFT if tasks should run.
bool runTaskIntake=false;             // controls INTAKE if tasks should run
bool runTaskWatchdog=false;           // control if we run the watchdog task

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

void watchdogTaskFnc(void* ignore) {
  //the void* is there to provide a way to pass a
  //generic value or structure to the task if needed
  //pros needs this parameter in your function even if you don't use it
  if(DEBUG) {std::cout << "Starting Watchdog task \n"; }
  while(true){
    while(runTaskWatchdog){
      // run the watchdog task - by setting runTaskWatchdog to flase at any
      // we can stop these task
      //if(DEBUG) { std::cout << "Running watchdog task \n"; }

      // we are going to monitor the drive train motors
      if(motorEnv) {
         //if(DEBUG) { std::cout << "Motor Environment Data \n";}
         // monitor and report motor environment data
         // we will monitor:
         //    power (Watts)
         //    torgue (Nm)
         //    voltage (mV)
         //    current (mA)
         //    temperature (C)
         std::cout << "Left Motor - P: " << left_wheel.get_power() << "W - T: " << left_wheel.get_torque();
         std::cout << "Nm - V: " << left_wheel.get_voltage() << "mV C: " << left_wheel.get_current_draw();
         std::cout << "mA - T: " << left_wheel.get_temperature() << "\n";

         if(left_wheel.get_temperature() > tempTrigger) {
           leftMotorHot = true;
         }

         std::cout << "Right Motor - P: " << right_wheel.get_power() << "W - T: " << right_wheel.get_torque();
         std::cout << "Nm - V: " << right_wheel.get_voltage() << "mV C: " << right_wheel.get_current_draw();
         std::cout << "mA - T: " << right_wheel.get_temperature() << "\n";

         if(right_wheel.get_temperature() > tempTrigger) {
           rightMotorHot = true;
         }

       }

       if(motorLiftEnv) {
          //if(DEBUG) { std::cout << "Motor Environment Data \n";}
          // monitor and report motor environment data
          // we will monitor:
          //    power (Watts)
          //    torgue (Nm)
          //    voltage (mV)
          //    current (mA)
          //    temperature (C)
          std::cout << "Lift Motor - P: " << liftMotor.get_power() << "W - T: " << liftMotor.get_torque();
          std::cout << "Nm - V: " << liftMotor.get_voltage() << "mV C: " << liftMotor.get_current_draw();
          std::cout << "mA - T: " << liftMotor.get_temperature() << "\n";

          if(liftMotor.get_temperature() > tempTrigger) {
            liftMotorHot = true;
          }

       }
    }
    pros::delay(20);          // do not starve the CPU
  }
  pros::delay(20);            // do not starve CPU
}
