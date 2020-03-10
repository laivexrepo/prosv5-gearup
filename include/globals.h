#ifndef GLOBALS_H_
#define GLOBALS_H_

#include "main.h"

// ------------ make sure all motors are available to all code -------------
extern pros::Motor left_wheel;
extern pros::Motor right_wheel;

// If drive based has second motor per side uncomment these statements
//extern pros::Motor left_wheel_2;
//extern pros::Motor right_wheel_2;

extern pros::Motor liftMotor;

extern pros::Motor trayMotor;

// Claw motors - if two motors used uncomment clawMotor2 statement
extern pros::Motor clawMotor1;
//extern pros::Motor clawMotor2;

extern pros::Controller master;
extern pros::Controller partner;

extern pros::Imu imu_sensor;

// ------------ Global accessible functions -------------------------------
extern void robotDataLogger();

// ----------- Global variable to control drive base mode -----------------
#define ARCADE_MODE false     // run in tankmode - if true arcade mode

// ----------- Global variables to assist in code development ------------
#define DEBUG true            // turn on DEBUG statements, should be off in final build

#define MANUAL_AUTON false     // allow for autonomous testing use Remote intead of
                              // competition switch -- set to false for production
                              // and competition!!!

// ---------- Global Task Variables ----------------------------------------

extern int intakeState;
extern int liftMoveAngle;
extern int liftMoveSpeed;
extern int liftLastMoveAngle;

extern bool motorEnv;
extern bool motorLiftEnv;

extern bool leftMotorHot;
extern bool rightMotorHot;
extern bool liftMotorHot;

// ................... watchdog trigger ........................................
extern int tempTrigger;

#endif
