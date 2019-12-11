#ifndef LIFT_H_
#define LIFT_H_

void liftControl(int speedUp, int speedDown);
void liftSetLowPreset();

float liftMoveForAngle(float angle, int speed );

extern float maxAngleMovement;
extern int liftGearRatio;

#endif
