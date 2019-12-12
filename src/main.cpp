#include "main.h"
#include "globals.h"
#include "portdef.h"
#include "drivebase.h"
#include "lift.h"
#include "claw.h"
#include "tray.h"
#include "autonomous.h"
#include "lcd.h"


/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {

	pros::Motor left_wheel (LEFT_MOTOR_PORT, MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
	pros::Motor right_wheel (RIGHT_MOTOR_PORT, MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);

	// Uncomment when using TWO motors per drive side
	//pros::Motor left_wheel_2 (LEFT_MOTOR_PORT_2, MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
	//pros::Motor right_wheel_2 (RIGHT_MOTOR_PORT_2, MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);

	pros::Motor liftMotor (LIFT_MOTOR_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);

	pros::Motor trayMotor (TRAY_MOTOR_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);

  pros::Motor clawMotor1(CLAW_MOTOR_PORT_1, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
  // uncomment below motor defintinos if using two claw (intake) motors -- second motr runs in oppostire direction
  //pros::Motor clawMotor2(CLAW_MOTOR_PORT_2, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);

  // Set the brake mode for the lift motor - hold position when stopped
	liftMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

	// Set the brake mode for the tray motor - hold position when stopped
	trayMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

  // Initialize the LCD display
	pros::delay(50);						// we need to give the LCD threat time to starts
															// otherwise the screen won't draw properly
	pros::lcd::initialize();
  pros::delay(20);						// We need the give the LCD call time to
															// complete.

	pros::lcd::register_btn0_cb(on_left_button);		// register callback buttons
	pros::lcd::register_btn1_cb(on_center_button);
	pros::lcd::register_btn2_cb(on_right_button);
	pros::delay(100);																// We need to give the CLD thread
																									// time to write to the screen

	if (!pros::competition::is_connected()) {
		if(DEBUG){ std::cout << "Not connected to FIELD control \n"; }
  } else {
		if(DEBUG){ std::cout << "Connected to FIELD control \n"; }
	}
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
	// We can put LCD selector here
	if(DEBUG){ std::cout << "Running Competition Initialize \n"; }

  // LEFT button toggles between 15sec, 45sec and 60sec autonomous routine
	// Set a global variable autonomousTime to either 15, 45 or 60
	//pros::lcd::print(1, "Press LEFT button to toggle Autonomous Time");
  pros::lcd::print(1, "Autonomous selected: %d", autonomousTime);

  // CENTER button sets ZERO points of all encoders for lift
  pros::lcd::print(2, "Press CENTER button to set ZERO point");

  // RIGHT button selects the autonomous starting point RED or BLUE
	pros::lcd::print(3, "Side selected: RED or BLUE");

}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
  pros::lcd::clear();												// CLEAR out the LCD display
	pros::delay(20);													// We need to give function time to complete

	// drive forward a given distance in cm.
	if(DEBUG){ std::cout << "Starting Autonomous Task \n"; }
	// we need to decide which autonomous routine/code to run, based
	// on the global variable autonomousTime

	switch(autonomousTime) {
		case 15:
			// Run the standard 15 sec autonomous code
			runStandardAuto();
			break;

		case 45:
		  // Run the 45 sec autonomous code
			runExtendedAuto();
			break;

		case 60:
			// Run the 60 seconds - programmign skill code
      runSkillAuto();
			break;
	}
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	if(DEBUG){ std::cout << "Starting Opcontrol Task \n"; }

  // call manual autonomous run option, should be commented out in
	// tournament production code to invertenly trigger autonmous
	// during driver control
	if(MANUAL_AUTON) {manualAutonomous(); }		// allow for manual autonmous starts
																						// define is does in globals.h

	pros::lcd::clear();												// CLEAR out the LCD display
  pros::delay(20);													// We need to give function time to complete

  while(true) {
	 	if(ARCADE_MODE) {												// If set to true operate in ARCADE_MODE
		 																				// this is controlled in globals.h
		 	arcadeControl();
	 	} else {
		  tankControl();
    }
	  // lift control takes two speeds, first speed is the up direction speed,
	  // the second is the down direction speed given in RPM
	  liftControl(100, 50);

    // control the intaqke (claw) if two motors - make sure they are
	  // activated in glabls.c globals.h initialize() as well as in the clawControl
	  // function in claw.cpp
	  clawControl();

	  // Control the movement of the tray inwards / outwards using appropriate speed
	  // for each movement.
	  trayControl(50,25);

 	  pros::delay(20);
  }
}
