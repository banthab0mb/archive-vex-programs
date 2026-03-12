#include "vex.h"
#include "sylib/sylib.hpp"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen.
brain  Brain;

// Main controller
controller Controller;

//The motor constructor takes motors as (port, ratio, reversed), so for example
//motor LeftFront = motor(PORT1, ratio6_1, false);

motor leftFront = motor(PORT11, ratio6_1, true);
motor leftMiddle = motor(PORT12, ratio6_1, true);
motor leftBack = motor(PORT13, ratio6_1, true);
motor rightFront = motor(PORT14, ratio6_1, false);
motor rightMiddle = motor(PORT15, ratio6_1, false);
motor rightBack = motor(PORT16, ratio6_1, false);

// Other devices

motor conveyor = motor(PORT17, ratio6_1, false);
motor conveyor2 = motor(PORT18, ratio18_1, false);
motor scorer = motor(PORT20, ratio18_1, true);

digital_out matchLoader = digital_out(Brain.ThreeWirePort.B);
digital_out hood = digital_out(Brain.ThreeWirePort.C);
digital_out park = digital_out(Brain.ThreeWirePort.D);
digital_out wings = digital_out(Brain.ThreeWirePort.E);
digital_out intakePiston = digital_out(Brain.ThreeWirePort.F);

distance parkSensor = distance(PORT4);

// Functions

void vexcodeInit( void ) {
  // nothing to initialize
}

bool noConveyor = true;
// Control for conveyor
void conveyorControl() {
  if(Controller.ButtonR1.pressing()) {
    conveyor.spin(forward, 12, volt);
    conveyor2.spin(forward, 200, rpm);
    noConveyor = false;
    scorer.spin(forward, 85, rpm);
  } else if(Controller.ButtonR2.pressing()) {
    conveyor.spin(reverse, 12, volt);
    conveyor2.spin(reverse, 200, rpm);
  } else {
    conveyor.stop(coast);
    conveyor2.stop(coast);
    noConveyor = true;
  }
}

bool userHoodState = false;
// Toggle for hood
void userHoodToggle() {
  userHoodState = !userHoodState;
  hood.set(userHoodState);
}

// Control for scorer
void scorerControl() {
  if(Controller.ButtonL1.pressing()) {
    scorer.spin(forward, 200, rpm);

  } else if(Controller.ButtonL2.pressing()) {
    scorer.spin(reverse, 200, rpm);
  } else {
    if (noConveyor) {
      scorer.stop();
    }
  }
}

// Pneumatic toggles

bool userLoaderState = false;
// Toggle for loader
void userLoaderToggle() {
  userLoaderState = !userLoaderState;
  matchLoader.set(userLoaderState);
}

bool userParkState = false;
// Control for double park
void userParkToggle() {
  userParkState = !userParkState;
  park.set(userParkState);
}

bool userWingsState = false;
// Toggle for wings
void userWingsToggle() {
  userWingsState = !userWingsState;
  wings.set(userWingsState);
}

bool userIntakeState = false;
// Toggle for intake piston
void userIntakeToggle() {
  userIntakeState = !userIntakeState;
  intakePiston.set(userIntakeState);
}