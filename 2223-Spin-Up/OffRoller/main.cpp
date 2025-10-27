#pragma region VEXcode Generated Robot Configuration
// Make sure all required headers are included.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>


#include "vex.h"

using namespace vex;

// Brain should be defined by default
brain Brain;


// START V5 MACROS
#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)
// END V5 MACROS


// Robot configuration code.
controller Controller1 = controller(primary);
motor front_left = motor(PORT9, ratio18_1, false);

motor front_right = motor(PORT2, ratio18_1, false);

motor back_right = motor(PORT6, ratio18_1, false);

motor back_left = motor(PORT17, ratio18_1, false);

motor flywheel = motor(PORT10, ratio18_1, true);

motor intakeroller = motor(PORT18, ratio18_1, false);

motor indexer = motor(PORT11, ratio18_1, false);

motor expansion = motor(PORT20, ratio18_1, false);

encoder EncoderC = encoder(Brain.ThreeWirePort.C);
led FWReady = led(Brain.ThreeWirePort.E);
line LineTrackerA = line(Brain.ThreeWirePort.A);
line LineTrackerB = line(Brain.ThreeWirePort.B);
inertial Inertial13 = inertial(PORT13);

limit IndexerLimit = limit(Brain.ThreeWirePort.F);
led FWNO = led(Brain.ThreeWirePort.G);
led indexback = led(Brain.ThreeWirePort.H);


// generating and setting random seed
void initializeRandomSeed(){
  int systemTime = Brain.Timer.systemHighResolution();
  double batteryCurrent = Brain.Battery.current();
  double batteryVoltage = Brain.Battery.voltage(voltageUnits::mV);

  // Combine these values into a single integer
  int seed = int(batteryVoltage + batteryCurrent * 100) + systemTime;

  // Set the seed
  srand(seed);
}



void vexcodeInit() {

  //Initializing random seed.
  initializeRandomSeed(); 
}


// Helper to make playing sounds from the V5 in VEXcode easier and
// keeps the code cleaner by making it clear what is happening.
void playVexcodeSound(const char *soundName) {
  printf("VEXPlaySound:%s\n", soundName);
  wait(5, msec);
}



// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;
// define variables used for controlling motors based on controller inputs
bool Controller1LeftShoulderControlMotorsStopped = true;

// define a task that will handle monitoring inputs from Controller1
int rc_auto_loop_function_Controller1() {
  // process the controller input every 20 milliseconds
  // update the motors based on the input values
  while(true) {
    if(RemoteControlCodeEnabled) {
      // check the ButtonL1/ButtonL2 status to control intakeroller
      if (Controller1.ButtonL1.pressing()) {
        intakeroller.spin(forward);
        Controller1LeftShoulderControlMotorsStopped = false;
      } else if (Controller1.ButtonL2.pressing()) {
        intakeroller.spin(reverse);
        Controller1LeftShoulderControlMotorsStopped = false;
      } else if (!Controller1LeftShoulderControlMotorsStopped) {
        intakeroller.stop();
        // set the toggle so that we don't constantly tell the motor to stop when the buttons are released
        Controller1LeftShoulderControlMotorsStopped = true;
      }
    }
    // wait before repeating the process
    wait(20, msec);
  }
  return 0;
}

task rc_auto_loop_task_Controller1(rc_auto_loop_function_Controller1);

#pragma endregion VEXcode Generated Robot Configuration

// ----------------------------------------------------------------------------
//                                                                            
//    Project: 344E X-Drive Code                                               
//    Author: Henry Rudowski
//    Created: 1/05/22
//    Configuration:        
//                                                                            
// ----------------------------------------------------------------------------

// Include the V5 Library
#include "vex.h"

// Allows for easier use of the VEX Library
using namespace vex;

// Begin project code


// ----------------------------------------------------------------------------
//                             FLYWHEEL PID SETUP
// ----------------------------------------------------------------------------

bool maintainSpeed = false;
double targetspeed = 4.5;
double kp = 0.5;
double ki = 0.10;
double kd = 0.05;
double kf = 2;
double preverror = 0.0;
double error = 0.0;
double totalError = 0.0; // += error
double derivative = 0.0; // = error-preverror
double flyspeed;
double Power = 0;
bool ReadyShoot = false;
int FlyPID(){
  while(maintainSpeed){
    flyspeed = (flywheel.velocity(rpm)); 
    error = targetspeed - flyspeed;
    if (error <= 0.1){
      ReadyShoot = true;
    }
    else{
      ReadyShoot = false;
    }
    //Power = (error*kp + totalError * ki + (error - preverror) * kd)/12;
    Power = targetspeed * kf;
    flywheel.spin(forward, Power, volt);
    preverror = error;
    totalError += error;
    vex::task::sleep(20);

  }
  flywheel.stop(coast);
  return 1;
}
// ----------------------------------------------------------------------------
//                     FUNCTION SETUPS & VARIABLE SETUPS
// ----------------------------------------------------------------------------

// flywheel functions
void shot () {
  maintainSpeed = true;
  FlyPID();
}
void noshot () {
  maintainSpeed = false;
}

bool velocup;

void veldown () {
  indexback.off();
  targetspeed = 4;
  velocup = false;
}
void velup () {
  indexback.on();
  targetspeed = 4.5;
  velocup = true;
}

// expansion function
void deploy () {
  expansion.spinFor(reverse, .5, turns);
}


// line tracker value print function
void printt() {
  // while (true) {
  // if (LineTrackerA.reflectivity() > LineTrackerB.reflectivity()) {
  //     //Controller1.Screen.clearLine(1);
  //     Controller1.Screen.setCursor(1, 15);
  //     Controller1.Screen.print("YES");
  //   } else {
  //     //Controller1.Screen.clearLine(1);
  //     Controller1.Screen.setCursor(1, 15);
  //     Controller1.Screen.print("NOT");
  //   }
  //   if (LineTrackerA.reflectivity() < LineTrackerB.reflectivity()) {
  //     //Controller1.Screen.clearLine(2);
  //     Controller1.Screen.setCursor(2, 15);
  //     Controller1.Screen.print("YES");
  //   } else {
  //     //Controller1.Screen.clearLine(2);
  //     Controller1.Screen.setCursor(2, 15);
  //     Controller1.Screen.print("NOT");
  //   }
  // }
}

bool shoot = false;

// Ready to shoot LED function

void shots() {
if (EncoderC.velocity(rpm) > 3000) {
    FWReady.on();
    FWNO.off();
    } else {
    FWReady.off();
    FWNO.on();
    }
}

// line cross corrector function
bool autonomouss = false;
void linee() {
while (autonomouss) {
    if (LineTrackerA.reflectivity() > LineTrackerB.reflectivity()) {
    Controller1.rumble("-");
    front_right.spinFor(reverse, 1, turns, false);
    front_left.spinFor(reverse, 1, turns, false);
    back_right.spinFor(forward, 1, turns, false);
    back_left.spinFor(forward, 1, turns, false);
    }
    if (LineTrackerA.reflectivity() < LineTrackerB.reflectivity()) {
    Controller1.rumble("-");
    front_right.spinFor(forward, 1, turns, false);
    front_left.spinFor(forward, 1, turns, false);
    back_right.spinFor(reverse, 1, turns, false);
    back_left.spinFor(reverse, 1, turns, false);
    }
  }
}


// ----------------------------------------------------------------------------
//                        COMPETITION FUNCTIONS & CODE
// ----------------------------------------------------------------------------


// ------------
//  PRE AUTON
// ------------
void preAutonomous(void) {
  // turn off leds
  FWReady.off();
  FWNO.on();
  indexback.off();
  // inertial sensor calibration
  Inertial13.calibrate();
  // Brain screen printing
  Brain.Screen.clearScreen(); 
  Brain.Screen.setFillColor(orange);
  Brain.Screen.drawRectangle(0, 0, 479, 239);
  Brain.Screen.setPenColor(white);
  Brain.Screen.setFillColor(black);
  Brain.Screen.drawRectangle(100, 50, 279, 139);
  Brain.Screen.setPenColor(white);
  // Brain.Screen.setFillColor(red);
  // Brain.Screen.drawRectangle(150, 75, 179, 89);
  Brain.Screen.setFont(prop60);
  Brain.Screen.setCursor(2, 7);
  Brain.Screen.setPenColor(yellow);
  Brain.Screen.print("344E");
  Brain.Screen.setFont(prop30);
  Brain.Screen.setCursor(5, 9);
  //Brain.Screen.setFillColor(black);
  Brain.Screen.setPenColor(red);
  Brain.Screen.print("Thunder Chickens");
  wait(1, seconds);
  // intake velocity
  intakeroller.setVelocity(200, rpm);
  // drive motors setup
  front_left.setStopping(brake);
  front_right.setStopping(brake);
  back_left.setStopping(brake);
  back_right.setStopping(brake);
  // flywheel motors setup
  //flywheel.setVelocity(100, rpm);
  indexer.setVelocity(200, rpm);
  flywheel.setStopping(coast);
  // intake & roller motor setup
 // intakeroller.setVelocity(100, rpm);
  // expansion motor setup
  expansion.setVelocity(100, rpm);
  expansion.setStopping(hold);
  expansion.stop(hold);
  // controller screen print
while (true) {
    //Controller1.Screen.clearLine(1);
    Controller1.Screen.setCursor(1, 5);
    Controller1.Screen.print(EncoderC.velocity(rpm));
    //Controller1.Screen.clearLine(2);
    //Controller1.Screen.clearLine(3);
    Controller1.Screen.setCursor(2, 5);
    Controller1.Screen.print(flywheel.temperature(temperatureUnits::fahrenheit));
    // wait(10, msec);
    //  Controller1.Screen.clearScreen();
    if (velocup == true) {
      Controller1.Screen.setCursor(4, 5);
      Controller1.Screen.clearLine(3);
      Controller1.Screen.print("UP");
    } else {
      Controller1.Screen.setCursor(4, 5);
      Controller1.Screen.clearLine(3);
      Controller1.Screen.print("DOWN");
    }
    wait(10, msec);
    Controller1.Screen.clearScreen();
  }

}
// ------------
//    AUTON
// ------------

void autonomous(void) {
  // AUTON
  autonomouss = true; 
  shoot = true;
front_left.setVelocity(100, rpm);
front_right.setVelocity(100, rpm);
back_right.setVelocity(100, rpm);
back_left.setVelocity(100, rpm);
// strafe roller auton
  front_right.spinFor(reverse, .1, turns, false);
  front_left.spinFor(reverse, .1, turns, false);
  back_right.spinFor(reverse, .1, turns, false);
  back_left.spinFor(reverse, .1, turns, true);
// strafe to roller
  front_right.spinFor(reverse, 2.3, turns, false);
  front_left.spinFor(reverse, 2.3, turns, false);
  back_right.spinFor(forward, 2.3, turns, false);
  back_left.spinFor(forward, 2.3, turns, true);

  front_right.spinFor(reverse, .4, turns, false);
  front_left.spinFor(forward, .4, turns, false);
  back_right.spinFor(reverse, .4, turns, false);
  back_left.spinFor(forward, .4, turns, true);
  wait(1, seconds);
  intakeroller.spinFor(forward, .4, turns, true);

  front_right.spinFor(forward, .2, turns, false);
  front_left.spinFor(reverse, .2, turns, false);
  back_right.spinFor(forward, .2, turns, false);
  back_left.spinFor(reverse, .2, turns, true);
  autonomouss = false;
  shoot = false;
}
// ------------
//    DRIVER
// ------------

void userControl(void) {
  shoot = true;
  // Driver control
  while (true) {
  // Drivetrain
    back_left.spin(forward, Controller1.Axis4.value() - Controller1.Axis1.value() + Controller1.Axis3.value(), percent);
    front_left.spin(forward, Controller1.Axis4.value() + Controller1.Axis1.value() + Controller1.Axis3.value(), percent);
    back_right.spin(forward, Controller1.Axis4.value() - Controller1.Axis1.value() - Controller1.Axis3.value(), percent);
    front_right.spin(forward, Controller1.Axis4.value() + Controller1.Axis1.value() - Controller1.Axis3.value(), percent);

  // Launcher
    //flywheel.setVelocity(350, rpm);
    // if(Controller1.ButtonR1.pressing()) {
    //   //flywheel.spin(forward, 5, volt);
    //   FlyPID ();
    // } else {
    //   //flywheel.stop();
    //   maintainSpeed = false;
    // }
    Controller1.ButtonR1.pressed(shot);
    Controller1.ButtonR2.pressed(noshot);

    Controller1.ButtonX.pressed(velup);
    Controller1.ButtonY.pressed(veldown);

    if(Controller1.ButtonUp.pressing()) {
      indexer.spin(forward);
    } else {
      indexer.stop();
    }
    
  // intake & roller
    if(Controller1.ButtonL1.pressing()) {
    intakeroller.spin(forward);
    } 
    if(Controller1.ButtonL2.pressing()) {
      intakeroller.spin(reverse);
    }

  // expansion
    Controller1.ButtonA.pressed(deploy);
   
   wait(20, msec);
  }
}

int main() {
  // create competition instance
  competition Competition;

  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(userControl);
  // Run the pre-autonomous function.
  preAutonomous();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}

/* void drivebot(double l, double r) {
front_right.spinFor(forward, r, turns, false);
front_left.spinFor(forward, l, turns, false);
back_right.spinFor(forward, r, turns, false);
back_left.spinFor(forward, l, turns, false);
}
void back(double l, double r)  {
front_right.spinFor(reverse, r, turns, false);
front_left.spinFor(reverse, l, turns, false);
back_right.spinFor(reverse, r, turns, false);
back_left.spinFor(reverse, l, turns, false);
}
void turnright(double l, double r) {
front_right.spinFor(reverse, r, turns, false);
front_left.spinFor(forward, l, turns, false);
back_right.spinFor(forward, r, turns, false);
back_left.spinFor(reverse, l, turns, false);
}
void turnleft(double l, double r)  {
front_right.spinFor(forward, r, turns, false);
front_left.spinFor(reverse, l, turns, false);
back_right.spinFor(reverse, r, turns, false);
back_left.spinFor(forward, l, turns, false);
} */