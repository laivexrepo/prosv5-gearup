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
#include "tasks.h"
#include "globals.h"


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

// ------------------ IMU ------------------------------------------------------

pros::Imu imu_sensor(IMU_PORT);

// ---------- Global Task Variables ----------------------------------------

int intakeState = 0;
int liftMoveAngle = 0;        // angle to move arm at in liftTask
int liftMoveSpeed = 50;       // speed to move lift at in liftTask
int liftLastMoveAngle = 0;    // last move angle requested in liftTask

bool motorEnv=true;           // watchdog reports motor environment data
bool motorLiftEnv=true;       // watchdog reports on liftmotor environment data

bool leftMotorHot=false;      // Overheating warning
bool rightMotorHot=false;     // Overheating warning
bool liftMotorHot=false;      // Overheating warning

// ................... watchdog trigger ........................................
int tempTrigger = 50;         // When do we trigger overheating - at what temp in C?


// --------------------- Global Function definitions ---------------------------
bool USDlogging=true;       // turn on USD card based logging

void usdWriteHeader(char* filename) {
  if(USDlogging) {
  	// first check if we got an USD card installed
    if(DEBUG) {std::cout << "USD Installed? " << pros::usd::is_installed() << "\n";}
  	if(pros::c::usd_is_installed()) {
  		// we got USD card
  		if(DEBUG) { std::cout << "USD card installed - preparing logging \n"; }
  	  // We need to open a file stream so we can write to USD card
      FILE* usd_file = fopen(filename, "w");
  		// Write CSV header as first line
      fputs("time,heading,X-accel,Y-accel,Z-accel,LM-P(W),LM-T(Nm),LM-V(mV),LM-I(mA),LM-T(C),", usd_file);
      fputs("LW-P(W),LW-T(Nm),LW-V(mV),LW-I(mA),LW-T(C),", usd_file);
      fputs("RW-P(W),RW-T(Nm),RW-V(mV),RW-I(mA),RW-T(C)\n", usd_file);
      fclose(usd_file);
  	} else {
  		// we got NO USD card installed
  		if(DEBUG) { std::cout << "USD card NOT installed - disabling logging \n"; }
      USDlogging=false;
  	}
  }
}

void robotDataLogger(char* filename) {
  // This function can be called and will write an entry to the dataLogger file
  // which is stored on the SD card and can record actions as the program runs
  // for later evaluation of performance
  if(DEBUG) { std::cout << "DataLogger Function Called \n";}
  if(USDlogging) {
  	// first check if we got an USD card installed
    if(DEBUG) {std::cout << "USD Installed? " << pros::usd::is_installed() << "\n";}
  	if(pros::c::usd_is_installed()) {
  		// we got USD card
  		if(DEBUG) { std::cout << "USD card installed - preparing logging \n"; }
  	  // We need to open a file stream so we can write to USD card
      FILE* usd_file = fopen(filename, "a");
      // Write the data points
      pros::c::imu_accel_s_t accel = imu_sensor.get_accel();
      fprintf(usd_file,"%d,", pros::millis());
      fprintf(usd_file,"%f,", imu_sensor.get_heading());
      fprintf(usd_file,"%f,%f,%f,",accel.x,accel.y,accel.z);
      // lift motor
      fprintf(usd_file,"%lf,",liftMotor.get_power());
      fprintf(usd_file,"%lf,",liftMotor.get_torque());
      fprintf(usd_file,"%d,",liftMotor.get_voltage());
      fprintf(usd_file,"%d,",liftMotor.get_current_draw());
      fprintf(usd_file,"%lf,",liftMotor.get_temperature());
      // left drive motor
      fprintf(usd_file,"%lf,",left_wheel.get_power());
      fprintf(usd_file,"%lf,",left_wheel.get_torque());
      fprintf(usd_file,"%d,",left_wheel.get_voltage());
      fprintf(usd_file,"%d,",left_wheel.get_current_draw());
      fprintf(usd_file,"%lf,",left_wheel.get_temperature());
      // right drive motor
      fprintf(usd_file,"%lf,",right_wheel.get_power());
      fprintf(usd_file,"%lf,",right_wheel.get_torque());
      fprintf(usd_file,"%d,",right_wheel.get_voltage());
      fprintf(usd_file,"%d,",right_wheel.get_current_draw());
      fprintf(usd_file,"%lf\n",right_wheel.get_temperature());
      fclose(usd_file);
  	} else {
  		// we got NO USD card installed
  		if(DEBUG) { std::cout << "USD card NOT installed - disabling logging \n"; }
      USDlogging=false;
  	}
  }


}
