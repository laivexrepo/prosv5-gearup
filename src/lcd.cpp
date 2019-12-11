#include "main.h"
#include "globals.h"
#include "portdef.h"
#include "lcd.h"
#include "lift.h"
#include "autonomous.h"

// left chooser button call back
// LEFT button selects the autonomous routine length it cycles through 3
// possibilities 15, 45, 60 it sets the global variable autonomousTime
void on_left_button() {
    switch(autonomousTime) {
      case 15:
        // code here
        autonomousTime = 45;
        break;

      case 45:
         // code here
         autonomousTime = 60;
         break;

      case 60:
        // code here
        autonomousTime = 15;
        break;
    }
    pros::lcd::print(1, "Autonomous selected: %d", autonomousTime);
    if(DEBUG) { std::cout << "Autonomous time selected: " << autonomousTime << "\n";}
}

// Center chooser button call back
void on_center_button(){
    pros::lcd::print(2, "Setting ZERO point for lift!");
    liftSetLowPreset();
}

// right chooser button call back
void on_right_button(){
  static std::string sideOfField;
  switch(fieldSide){
    case 1:                   // RED side
      if(DEBUG) { std::cout << "Switching to BLUE field side \n"; }
      fieldSide = 2;
      sideOfField = "BLUE";
      break;

    case 2:
      if(DEBUG) { std::cout << "Switching to RED field side \n"; }
      fieldSide = 1;
      sideOfField = "RED";
      break;
  }
  pros::lcd::print(3, "Field side selected: %s", sideOfField);
  if(DEBUG) { std::cout << "Field side selected: " << sideOfField << "\n";}
}
