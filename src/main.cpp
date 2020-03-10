#include "main.h"
#include "globals.h"
#include "portdef.h"
#include "drivebase.h"
#include "lift.h"
#include "claw.h"
#include "tray.h"
#include "autonomous.h"
#include "lcd.h"
#include "tasks.h"
#include "imu.h"

// Version 1.6 -- IMU with accelerometer

// Start the various Autonomus tasks to allow "parallel" operation of mechanisms
pros::Task intakeTask(intakeTaskFnc, (void*)"PROS", TASK_PRIORITY_DEFAULT,
							TASK_STACK_DEPTH_DEFAULT, "Intake Task"); //starts the task
// no need to provide any other parameters

pros::Task liftTask(liftTaskFnc, (void*)"PROS", TASK_PRIORITY_DEFAULT,
							TASK_STACK_DEPTH_DEFAULT, "Lift Task"); //starts the task
// no need to provide any other parameters

pros::Task watchdogTask(watchdogTaskFnc, (void*)"PROS", TASK_PRIORITY_DEFAULT,
							TASK_STACK_DEPTH_DEFAULT, "Watchdog Task"); //starts the task
// no need to provide any other parameters

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

	// Set the brake mode for the claw motor - hold position when stopped
	clawMotor1.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

	// Initialize the LCD display
	pros::delay(50);						// we need to give the LCD threat time to starts
															// otherwise the screen won't draw properly
	pros::lcd::initialize();
	pros::delay(50);						// We need the give the LCD call time to
															// complete.

	pros::lcd::register_btn0_cb(on_left_button);		// register callback buttons
	pros::lcd::register_btn1_cb(on_center_button);
	pros::lcd::register_btn2_cb(on_right_button);
	pros::delay(100);																// We need to give the CLD thread
																									// time to write to the screen
  // initialize inertial sensor and calibrate
  pros::Imu imu_sensor(IMU_PORT);

	imu_sensor.reset();				// Reset the IMU and start calibration process, robot
														// should not be moved during this stage of calibration.

	int time = pros::millis();
	int iter = 0;
	while (imu_sensor.is_calibrating()) {
		std::cout << "IMU calibrating... " << iter << "\n";
		iter += 10;
		pros::delay(10);
	}
	// should print about 2000 ms
	std::cout << "IMU is done calibrating (took: " << iter - time << "ms \n";

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
	if(DEBUG) {
		std::cout << "Are the two required Tasks running? \n";
		std::cout << "Task Name:" << intakeTask.get_name() << "\n";
		std::cout << "Task Name:" << liftTask.get_name() << "\n";
	}

  runTaskWatchdog = false;      // in atuonmous don't run watchdog task

  pros::delay(30);

	// drive forward a given distance in cm.
	if(DEBUG){ std::cout << "Starting Autonomous Task \n"; }

	if(runTaskLift) { std::cout << "runTaskLift is TRUE \n";} else { std::cout << "runTaskLift is FALSE \n";}
	if(runTaskIntake) { std::cout << "runTaskIntake is TRUE \n";} else { std::cout << "runTaskIntake is FALSE \n";}
	if(runTaskWatchdog) { std::cout << "runTaskWatchdog is TRUE \n";} else { std::cout << "runTaskWatchdog is FALSE \n";}


	// we need to decide which autonomous routine/code to run, based
	// on the global variable autonomousTime

	switch(autonomousTime) {
		default:									// If we have no selection defualt to 15 seconds
		case 15:
			// Run the standard 15 sec autonomous code
			if(DEBUG){ std::cout << "Starting runStandardAuto function  \n"; }
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

	// IMU test routines - can only run one of these at the time
	//imuHeadingTest();				// test Gyro
  //imuAccelerometerTest();		// test accelerometer

  // call manual autonomous run option, should be commented out in
	// tournament production code to invertenly trigger autonmous
	// during driver control
	if(MANUAL_AUTON) {manualAutonomous(); }		// allow for manual autonmous starts
																						// define is does in globals.h

  runTaskLift = false;											// ensure manually started tasks are Ended
  runTaskIntake = false;
	runTaskWatchdog = true;      							// in opcontrol run watchdog task

	pros::lcd::clear();												// CLEAR out the LCD display
  pros::delay(20);													// We need to give function time to complete

	if(runTaskLift) { std::cout << "runTaskLift is TRUE \n";} else { std::cout << "runTaskLift is FALSE \n";}
	if(runTaskIntake) { std::cout << "runTaskIntake is TRUE \n";} else { std::cout << "runTaskIntake is FALSE \n";}
	if(runTaskWatchdog) { std::cout << "runTaskWatchdog is TRUE \n";} else { std::cout << "runTaskWatchdog is FALSE \n";}

  int count = 0;													// while loop iteration counter

	// FOR testing
	leftMotorHot=true;

  while(true) {
		// environment monitoring usign watchdog task
		if(leftMotorHot) {
  		// Lets write warning to Remote
	  	if(DEBUG) { std::cout << "OVERHEATING WARING -- left drive motor \n";}
      if(!(count % 5)) {master.set_text(1, 0, "HOT DRIVE MOTOR");}
		}
		if(rightMotorHot) {
			// Lets write warning to Remote
			if(DEBUG) { std::cout << "OVERHEATING WARING -- right drive motor \n";}
			if(!(count % 5)) {master.set_text(1, 0, "HOT DRIVE MOTOR");}
	  }
		if(liftMotorHot) {
			// Lets write warning to Remote
			if(DEBUG) { std::cout << "OVERHEATING WARING -- lift motor \n";}
			if(!(count % 5)) {master.set_text(1, 0, "HOT LIFT MOTOR");}
		}

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

    count++;
 	  pros::delay(20);
  }
}
