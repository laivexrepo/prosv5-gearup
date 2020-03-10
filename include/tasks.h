#ifndef TASKS_H_
#define TASKS_H_

extern void intakeTaskFnc(void* ignore);
extern void liftTaskFnc(void* ignore);
extern void watchdogTaskFnc(void* ignore);

extern bool runTaskLift;
extern bool runTaskIntake;
extern bool runTaskWatchdog;

#endif
