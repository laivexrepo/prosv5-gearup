#include "main.h"
#include "globals.h"
#include "portdef.h"
#include "tray.h"

void trayControl(int speedIn, int speedOut){
  // speedIn is the unwards movement - moving tray backinto robot frame speed,
  // speedOut is the outwards movement speed - moving tray out to stack the stack
  // onto field/in goal
  if (master.get_digital(DIGITAL_UP)) {
     trayMotor.move_velocity(speedIn);
   }
   else if (master.get_digital(DIGITAL_DOWN)) {
     trayMotor.move_velocity(-speedOut);
   }
   else {
     trayMotor.move_velocity(0);
   }
}
