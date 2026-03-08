#include "vex.h"
#include "sylib/sylib.hpp"
#include "images/brain_banner.h"
#include <cmath>
#include <cstdint>
#include <string>
#include <vector>

using namespace vex;
competition Competition;

// LED devices

auto leftUnderglow = sylib::Addrled(22, 7, 11);
auto rightUnderglow = sylib::Addrled(22, 8, 11);

char currentTeam =  ' ';

// Chassis constructor

// robor
Drive chassis(

//Pick your drive setup from the list below:
//ZERO_TRACKER_NO_ODOM
//ZERO_TRACKER_ODOM
//TANK_ONE_FORWARD_ENCODER
//TANK_ONE_FORWARD_ROTATION
//TANK_ONE_SIDEWAYS_ENCODER
//TANK_ONE_SIDEWAYS_ROTATION
//TANK_TWO_ENCODER
//TANK_TWO_ROTATION
//HOLONOMIC_TWO_ENCODER
//HOLONOMIC_TWO_ROTATION
//
//Write it here:
ZERO_TRACKER_ODOM,

//Add the names of your Drive motors into the motor groups below, separated by commas, i.e. motor_group(Motor1,Motor2,Motor3).
//You will input whatever motor names you chose when you configured your robot using the sidebar configurer, they don't have to be "Motor1" and "Motor2".

//Left Motors:
motor_group(leftFront,leftMiddle,leftBack),

//Right Motors:
motor_group(rightFront,rightMiddle,rightBack),

//Specify the PORT NUMBER of your inertial sensor, in PORT format (i.e. "PORT1", not simply "1"):
PORT9,

//Input your wheel diameter. (4" omnis are actually closer to 4.125"):
3.25,

//External ratio, must be in decimal, in the format of input teeth/output teeth.
//If your motor has an 84-tooth gear and your wheel has a 60-tooth gear, this value will be 1.4.
//If the motor drives the wheel directly, this value is 1:
0.75,

//Gyro scale, this is what your gyro reads when you spin the robot 360 degrees.
//For most cases 360 will do fine here, but this scale factor can be very helpful when precision is necessary.
360,

/*---------------------------------------------------------------------------*/
/*                                  PAUSE!                                   */
/*                                                                           */
/*  The rest of the drive constructor is for robots using POSITION TRACKING. */
/*  If you are not using position tracking, leave the rest of the values as  */
/*  they are.                                                                */
/*---------------------------------------------------------------------------*/

//If you are using ZERO_TRACKER_ODOM, you ONLY need to adjust the FORWARD TRACKER CENTER DISTANCE.

//FOR HOLONOMIC DRIVES ONLY: Input your drive motors by position. This is only necessary for holonomic drives, otherwise this section can be left alone.
//LF:      //RF:    
PORT1,     -PORT2,

//LB:      //RB: 
PORT3,     -PORT4,

//If you are using position tracking, this is the Forward Tracker port (the tracker which runs parallel to the direction of the chassis).
//If this is a rotation sensor, enter it in "PORT1" format, inputting the port below.
//If this is an encoder, enter the port as an integer. Triport A will be a "1", Triport B will be a "2", etc.
PORT11,

//Input the Forward Tracker diameter (reverse it to make the direction switch):
2.75,

//Input Forward Tracker center distance (a positive distance corresponds to a tracker on the right side of the robot, negative is left.)
//For a zero tracker tank drive with odom, put the positive distance from the center of the robot to the right side of the drive.
//This distance is in inches:
6.75,

//Input the Sideways Tracker Port, following the same steps as the Forward Tracker Port:
PORT11,

//Sideways tracker diameter (reverse to make the direction switch):
-2,

//Sideways tracker center distance (positive distance is behind the center of the robot, negative is in front):
-1.5

);

int current_auton_selection = 0;
bool auto_started = false;

/**
 * Function before autonomous. 
 * It prints the current auton number on the screen and 
 * tapping the screen cycles the selected auton by 1. 
 */

void pre_auton() {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  default_constants();

  // Vibrate controller when init is done
  Controller.rumble("-");

  while(!auto_started){
    
    Brain.Screen.clearScreen();
    Brain.Screen.setPenColor(white);
    Brain.Screen.printAt(5, 20, "JAR Template v1.2.0");
    Brain.Screen.printAt(5, 40, "Battery Percentage:");
    Brain.Screen.printAt(5, 60, "%d", Brain.Battery.capacity());
    Brain.Screen.printAt(5, 80, "Chassis Heading Reading:");
    Brain.Screen.printAt(5, 100, "%f", chassis.get_absolute_heading());
    Brain.Screen.printAt(5, 120, "Selected Auton:");

    // auton selector
    switch(current_auton_selection){
      case 0:
        Brain.Screen.setPenColor(red);
        Brain.Screen.printAt(5, 140, "Red Drive Forward");
        currentTeam = 'R';
        break;
      case 1:
        Brain.Screen.setPenColor(red);
        Brain.Screen.printAt(5, 140, "Red Left");
        currentTeam = 'R';
        break;
      case 2:
        Brain.Screen.setPenColor(red);
        Brain.Screen.printAt(5, 140, "Red Right");
        currentTeam = 'R';
        break;
      case 3:
        Brain.Screen.setPenColor(red);
        Brain.Screen.printAt(5, 140, "Red Solo AWP");
        currentTeam = 'R';
        break;
      case 4:
        Brain.Screen.setPenColor(blue);
        Brain.Screen.printAt(5, 140, "Blue Drive Forward");
        currentTeam = 'B';
        break;
      case 5:
        Brain.Screen.setPenColor(blue);
        Brain.Screen.printAt(5, 140, "Blue Left");
        currentTeam = 'B';
        break;
      case 6:
        Brain.Screen.setPenColor(blue);
        Brain.Screen.printAt(5, 140, "Blue Right");
        currentTeam = 'B';

        break;
      case 7:
        Brain.Screen.setPenColor(blue);
        Brain.Screen.printAt(5, 140, "Blue Solo AWP");
        currentTeam = 'B';
        break;
      case 8:
        Brain.Screen.setPenColor(green); 
        Brain.Screen.printAt(5, 140, "Skills");
        currentTeam = 'S';
        break;
    }
    if(Brain.Screen.pressing()){
      while(Brain.Screen.pressing()) {}
      current_auton_selection ++;
    } else if (current_auton_selection == 9){
      current_auton_selection = 0;
    }
    sylib::delay(10);
  }
}

/**
 * Auton function, which runs the selected auton. Case 0 is the default,
 * and will run in the brain screen goes untouched during preauton. 
 */

void autonomous(void) {
  auto_started = true;

  if (currentTeam == 'R') { // if on RED alliance, set LEDs to RED
    leftUnderglow.set_all(0xFF0000);
    rightUnderglow.set_all(0xFF0000);
  } else if (currentTeam == 'B') { // if on BLUE alliance, set LEDs to BLUE
    leftUnderglow.set_all(0x0000FF);
    rightUnderglow.set_all(0x0000FF);
  } else { // for skills set LEDs to rainbow
    leftUnderglow.gradient(0xFF0000, 0xFF0005, 0, 0, false, true);
    leftUnderglow.cycle(*leftUnderglow, 10);
    rightUnderglow.gradient(0xFF0000, 0xFF0005, 0, 0, false, true);
    rightUnderglow.cycle(*rightUnderglow, 10);
  }

  // Display brain banner image on brain screen
  Brain.Screen.drawImageFromBuffer((uint8_t*)brain_banner, 0, 0, sizeof(brain_banner));

  switch(current_auton_selection){ 
    case 0:
      drive_forward();
      break;
    case 1:         
      red_left();
      break;
    case 2:
      red_right();
      break;
    case 3:
      red_solo_awp();
      break;
    case 4:
      drive_forward();
      break;
    case 5:
      blue_left();
      break;
    case 6:
      blue_right();
      break;
    case 7:
      blue_solo_awp();
      break;
    case 8:
      skills();
      break;
 }
}

// User Control Task

void usercontrol(void) {
  
  // Set underglow to rainbow if no autonomous has run
  // This makes sure the colors set in auto stay
  if (!Competition.isFieldControl() && auto_started == false) {
    // underglow rainbow cycle
    leftUnderglow.gradient(0xFF0000, 0xFF0005, 0, 0, false, true);
    leftUnderglow.cycle(*leftUnderglow, 10);
    rightUnderglow.gradient(0xFF0000, 0xFF0005, 0, 0, false, true);
    rightUnderglow.cycle(*rightUnderglow, 10);
  }

  // Tells the brain screen printing in preAuton() to stop
  auto_started = true;

  // Display brain banner image on brain screen

  Brain.Screen.drawImageFromBuffer((uint8_t*)brain_banner, 0, 0, sizeof(brain_banner));

  // Controller button callbacks
  Controller.ButtonDown.pressed(userLoaderToggle);
  Controller.ButtonUp.pressed(userParkToggle);
  Controller.ButtonY.pressed(userHoodToggle);
  Controller.ButtonB.pressed(userWingsToggle);
  Controller.ButtonRight.pressed(userIntakeToggle);

  std::uint32_t clock = sylib::millis();
  while (1) {
    //Replace this line with chassis.control_tank(); for tank drive 
    //or chassis.control_holonomic(); for holo drive.

    chassis.control_arcade(); // Standard arcade with deadband

    //cubic(); // Cubic drive

    //quadratic(); // Quadratic drive

    // Motor controls
    conveyorControl();
    scorerControl();

    // 10ms delay to allow other tasks to run
    sylib::delay_until(&clock, 10);
  }
}

//
// No touchy
//
int main() {

  // init sylib
  sylib::initialize();

  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
