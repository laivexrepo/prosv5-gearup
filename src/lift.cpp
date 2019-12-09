#include "main.h"
#include "globals.h"
#include "portdef.h"
#include "lift.h"

void liftControl(int speedUp, int speedDown){
  if (master.get_digital(DIGITAL_R1)) {
     liftMotor.move_velocity(speedUp);
   }
   else if (master.get_digital(DIGITAL_R2)) {
     liftMotor.move_velocity(-speedDown);
   }
   else {
     liftMotor.move_velocity(0);
   }
}
