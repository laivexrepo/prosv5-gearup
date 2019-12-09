#include "main.h"
#include "globals.h"
#include "portdef.h"
#include "claw.h"

void clawControl(){
  if (master.get_digital(DIGITAL_L1)) {
     clawMotor1.move_velocity(50);
     // If two intake (claw) motors uncomment this - motor spins by default
     // opposite direction of clawMotor1
     //clawMotor2.move_velocity(50);
   }
   else if (master.get_digital(DIGITAL_L2)) {
     clawMotor1.move_velocity(-50);
     // If two intake (claw) motors uncomment this - motor spins by default
     // opposite direction of clawMotor1
     //clawMotor2.move_velocity(-50);
   }
   else {
     clawMotor1.move_velocity(0);
     // If two intake (claw) motors uncomment this - motor spins by default
     // opposite direction of clawMotor1
     //clawMotor2.move_velocity(0);
   }
}
