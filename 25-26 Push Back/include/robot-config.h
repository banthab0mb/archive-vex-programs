#pragma once

#include <cmath>
#include <cstdint>
#include <string>
#include <vector>

using namespace vex;

extern brain Brain;

extern controller Controller;

// Drive motors

extern motor leftFront;
extern motor leftMiddle;
extern motor leftBack;
extern motor rightFront;
extern motor rightMiddle;
extern motor rightBack;
extern motor_group wholeDrivetrain;

// Devices

extern motor conveyor;
extern motor conveyor2;
extern motor scorer;

extern digital_out matchLoader;
extern digital_out hood;
extern digital_out park;
extern digital_out wings;
extern digital_out intakePiston;

extern distance parkSensor;

// Function

void vexcodeInit( void );

void conveyorControl();
void scorerControl();

void userLoaderToggle();
void userHoodToggle();
void userWingsToggle();
void userParkToggle();
void userIntakeToggle();