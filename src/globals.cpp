// ------- globals.cpp ---------------------------------------------------------
//
// Use globals.cpp together with globals.h to define all motor and other objects
// which should be available/accessible in autonomous and opcontrol code.
//
// forexample setup a motor definition and then add a extern pros::motor to
// globals.h to ensure all other modules have access -- of course by including
// #include "globals.h" in the relevant source (.cpp) files

#include "main.h"
#include "portdef.h"


// --------------------- Global Motor Definitions ------------------------------

pros::Motor left_wheel (LEFT_MOTOR_PORT, MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor right_wheel (RIGHT_MOTOR_PORT, MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);

// If drivebase has second motor perside, uncomment the below defintions (make sure you also do in globals.h)
//pros::Motor left_wheel_2 (LEFT_MOTOR_PORT_2, MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
//pros::Motor right_wheel_2 (RIGHT_MOTOR_PORT_2, MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor liftMotor(LIFT_MOTOR_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor clawMotor1(CLAW_MOTOR_PORT_1, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
// if two claw or intake motors uncomment below statement (also in global.h) please not that this
// second motor runs in opposite direction as clawMotor1
//pros::Motor clawMotor2(CLAW_MOTOR_PORT_2, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);

pros::Motor trayMotor(TRAY_MOTOR_PORT, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);

// -------------------- Remote Controls -----------------------------------------

pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::Controller partner(pros::E_CONTROLLER_PARTNER);

// ------------------- Vision Sensor -------------------------------------------

pros::Vision vision_sensor (VISION_PORT);

// --------------------- Global Function definitions ---------------------------

void robotDataLogger() {
  // This function can be called and will write an entry to the dataLogger file
  // which is stored on the SD card and can record actions as the program runs
  // for later evaluation of performance

}
