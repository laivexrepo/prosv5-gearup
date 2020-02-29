#include "autonomous.h"
#include "portdef.h"
#include "globals.h"
#include "drivebase.h"
#include "lift.h"
#include "tasks.h"
#include "claw.h"
#include "imu.h"

int autonomousTime = 15;    // 15sec 45sec 60sec autonomous run

int fieldSide=1;            // fieldside 1 = RED 2 = BLUE

// provide a button function to start autonomous manual from controller for testing
void manualAutonomous(){
  // place this function in the top of the opcontrol function in main.cpp
  if(DEBUG){ std::cout << "Entered Manual Autonomous Control Function \n"; }

  // because when no competition switch is connected, we will never reach
  // the competition_initialize thread, we need to facilitate the ability
  // to select settigns on the LCD screen
  bool selectLCD = true;
  if(DEBUG){ std::cout << "Starting LCD select routine \n"; }

  // LEFT button toggles between 15sec, 45sec and 60sec autonomous routine
	// Set a global variable autonomousTime to either 15, 45 or 60
  pros::lcd::print(1, "Autonomous selected: %d", autonomousTime);

  // CENTER button sets ZERO points of all encoders for lift
  pros::lcd::print(2, "Press CENTER button to set ZERO point");

  // RIGHT button selects the autonomous starting point RED or BLUE
	pros::lcd::print(3, "Side selected: RED or BLUE");

  pros::delay(100); // allow time to write to LCD

  while(selectLCD) {
    if (master.get_digital(DIGITAL_X)) {
      selectLCD = false;
      if(DEBUG){ std::cout << "Ending LCD select routine \n"; }
    }
    pros::delay(100);     // we need to give LCD time to update screen
  }

  bool autoCheck=true;
  while (autoCheck) {
     if (master.get_digital(DIGITAL_A)) {
        // If the button A is pushed on the master remote, we will call Autonomous
        // function
        runTaskIntake=true;       // start task in manual autonomous
        runTaskLift=true;

        if(DEBUG){ std::cout << "Starting Autonomous Task \n"; }
        if(runTaskLift) { std::cout << "runTaskLift is TRUE \n";} else { std::cout << "runTaskLift is FALSE \n";}
        if(runTaskIntake) { std::cout << "runTaskIntake is TRUE \n";} else { std::cout << "runTaskIntake is FALSE \n";}

        autonomous();        // Run autonomous manual
        autoCheck=false;
    }
    if (master.get_digital(DIGITAL_Y)) {
       // Drop out of autonomous check when the Y button is pressed
       autoCheck=false;
       runTaskIntake=false;       // ensure tasks end
       runTaskLift=false;

       if(DEBUG){ std::cout << "Ended Manual Autonomus Task \n"; }
    }
  }
}

void runStandardAuto() {
  // Standard 15 seconds autonomous
  intakeState = 1;            // Start running intake forward,
                              // untill we change the intakeState - this controls
                              // the intakeTaskFnc tasks behaviour

  float angleOfTurn;

  if(runTaskLift) { std::cout << "runStandardAuto: runTaskLift is TRUE \n";} else { std::cout << "runTaskLift is FALSE \n";}
  if(runTaskIntake) { std::cout << "runStandardAuto: runTaskIntake is TRUE \n";} else { std::cout << "runTaskIntake is FALSE \n";}

  //angleOfTurn = pivotForAngleWithIMU(90, 100);

  //if(DEBUG) { std::cout << "Angle of turn: " << angleOfTurn << " Current Heading: " << imu_sensor.get_heading() << " \n"; }
  //pros::delay(1000);            // wait a second

  //angleOfTurn = pivotForAngleWithIMU(-90, 15);

  //if(DEBUG) { std::cout << "Angle of turn: " << angleOfTurn << " Current Heading: " << imu_sensor.get_heading() << " \n"; }


  driveForDistance(100, 50);  // 100cm forward

  liftMoveAngle = 45;         // Tell lift task to move arm
  liftMoveSpeed = 50;

  driveForDistance(-100, 50); // 100cm backwards

  // pivot turn
  pivotTurn(90, 50);					// 90degree clockwise at 50RPM
  pivotTurn(-90,50);					// 90 degree coutner clockwise at 50RPM

  driveForDistance(50, 100);  // 50cm forward  at 100RPM

  swingTurn(90, 50);					// 90 degree swing turn at 50RPM clockwise
  swingTurn(-90, 50);					// 90 degree swing turn at 50RPM counter clockwise


}

void runExtendedAuto(){
  // Run the extended 45sec autonomous code
  // Lets run different code based on RED or BLUE side, the global variable
  // fieldSide lets us know this 1 == RED 2 == blue
  if(fieldSide == 1) {
    if(DEBUG) { std::cout << "RED side selected for 45sec autonomous \n "; }
  }

  if(fieldSide == 2) {
    if(DEBUG) { std::cout << "BLUE side selected for 45sec autonomous \n "; }
  }
}

void runSkillAuto(){
  float armAngle = 0;
  // Run the skill challenge (programming skill code)
  armAngle = liftMoveForAngle(45, 50 );        // move up 45degree from Zero point
  if(DEBUG) { std::cout << "Arm angle after move: " << armAngle << " \n";}
  pros::delay(5000);               // 5 sec (5000mSec) delay for testing purpose
  armAngle = liftMoveForAngle(0, 50 );         // return to ZERO point
  if(DEBUG) { std::cout << "Arm angle after move: " << armAngle << " \n";}
}
