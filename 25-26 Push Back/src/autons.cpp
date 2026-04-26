#include "vex.h"
#include "sylib/sylib.hpp"

/**
 * Resets the constants for auton movement.
 * Modify these to change the default behavior of functions like
 * drive_distance(). For explanations of the difference between
 * drive, heading, turning, and swinging, as well as the PID and
 * exit conditions, check the docs.
 */

void default_constants(){
  // Each constant set is in the form of (maxVoltage, kP, kI, kD, startI).
  chassis.set_drive_constants(10, 0.9, 0, 5.15, 0);
  chassis.set_heading_constants(6, .370, 0.005, 2.8, 15);
  chassis.set_turn_constants(6, .54, 0.01, 4.434, 0);
  chassis.set_swing_constants(12, .437, .0295, 3.486, 15);

  // Each exit condition set is in the form of (settle_error, settle_time, timeout).
  chassis.set_drive_exit_conditions(1.5, 200, 2000);
  chassis.set_turn_exit_conditions(1.5, 200, 1000);
  chassis.set_swing_exit_conditions(1, 200, 3000);
}

/**
 * Sets constants to be more effective for odom movements.
 * For functions like drive_to_point(), it's often better to have
 * a slower max_voltage and greater settle_error than you would otherwise.
 */

void odom_constants(){
  default_constants();
  chassis.heading_max_voltage = 10;
  chassis.drive_max_voltage = 8;
  chassis.drive_settle_error = 3;
  chassis.boomerang_lead = .5;
  chassis.boomerang_setback = 2;
  chassis.drive_min_voltage = 2;
}

bool loaderState = false;
// Toggle for loader
void loaderToggle() {
  loaderState = !loaderState;
  matchLoader.set(loaderState);
}

bool parkState = false;
// Toggle for double park
void parkToggle() {
  parkState = !parkState;
  park.set(parkState);
}

bool hoodState = false;
// Toggle for hood
void hoodToggle() {
  hoodState = !hoodState;
  hood.set(hoodState);
}

bool wingsState = false;
// Toggle for wings
void wingsToggle() {
  wingsState = !wingsState;
  wings.set(wingsState);
}

// Drives forward X inches
void drive_forward(){
  //chassis.set_coordinates(0,0,0);
  //chassis.turn_to_angle(90);
  chassis.drive_distance(7);
}

// Solo AWP on red side
void red_solo_awp(){
  // win point auto, 
  odom_constants();

  chassis.set_coordinates(-46.274, -14.764, 0);
  chassis.drive_max_voltage = 12;
  //parkToggle();

  chassis.drive_to_pose(-46.561, -46.859, 0);
  chassis.turn_to_angle(270);
  loaderToggle(); //loader down
  chassis.drive_max_voltage = 4;
  conveyor.spin(forward, 12, volt);
  conveyor2.spin(forward, 200, rpm);
  scorer.spin(forward, 85, rpm);
  chassis.drive_to_pose(-59.169, -47.145, 270); //at loader
  sylib::delay(500);
  conveyor.stop();
  conveyor2.stop();
  scorer.stop();
  chassis.drive_max_voltage = 12;
  chassis.drive_to_pose(-31.086, -47.145, 270); // at long goal
  conveyor.spin(forward, 12, volt);
  conveyor2.spin(forward, 200, rpm);
  loaderToggle(); //loader up
  hoodToggle(); //hood up
  scorer.spin(forward, 200, rpm);
  sylib::delay(1000); //unloading time
  hoodToggle(); // hood down
  chassis.drive_to_pose(-32.233, -33.39, 75);
  conveyor.spin(forward, 12, volt);
  conveyor2.spin(forward, 200, rpm);
  scorer.spin(forward, 85, rpm);
  chassis.drive_to_pose(-13.319, -28.519, 75);
  chassis.drive_max_voltage = 4;
  chassis.turn_to_angle(300);
  chassis.drive_to_pose(-30.227, -15.91, 300); // pick up 3 stack
  conveyor.stop();
  conveyor2.stop();
  scorer.stop();
  chassis.drive_to_pose(-11.887, -11.898, 45); // go to bottom middle goal
  conveyor.spin(reverse, 12, volt);
  conveyor2.spin(reverse, 200, rpm);
  scorer.spin(reverse, 85, rpm);
  sylib::delay(1500); //unloading time
  conveyor.stop();
  conveyor2.stop();
  scorer.stop();
  chassis.drive_max_voltage = 12;
  chassis.drive_to_pose(-37.677, -4.161, 0);
  chassis.drive_to_pose(33.665, 14.179, 45);
  chassis.drive_max_voltage = 4;
  conveyor.spin(forward, 12, volt);
  conveyor2.spin(forward, 200, rpm);
  scorer.spin(forward, 85, rpm);
  chassis.drive_to_pose(-12.746, 29.367, 45); // picked up second 3 stack
  conveyor.stop();
  conveyor2.stop();
  scorer.stop();
  chassis.drive_to_pose(-9.881, 9.594, 315); // go to top middle goal
  scorer.spin(reverse, 200, rpm); //unload
  sylib::delay(100);
  conveyor.spin(forward, 12, volt);
  conveyor2.spin(forward, 200, rpm);
  sylib::delay(1000);
  conveyor.stop();
  conveyor2.stop();
  scorer.stop();
  chassis.drive_max_voltage = 12;
  chassis.drive_to_pose(-47.134, 46.847, 315);
  chassis.turn_to_angle(270);
  loaderToggle(); //loader down
  conveyor.spin(forward, 12, volt);
  conveyor2.spin(forward, 200, rpm);
  scorer.spin(forward, 85, rpm);
  chassis.drive_max_voltage = 4;
  chassis.drive_to_pose(-57.737, 46.847, 270); // at loader
  sylib::delay(500);
  conveyor.stop();
  conveyor2.stop();
  scorer.stop();
  chassis.drive_max_voltage = 12;
  chassis.drive_to_pose(-30.8, 46.847, 270); // at long goal
  hoodToggle();
  conveyor.spin(forward, 12, volt);
  conveyor2.spin(forward, 200, rpm);
  scorer.spin(forward, 200, rpm);
}

/** 
 * Solo AWP on blue side
 * Calls the corresponding red side function 
 * b/c of symmetrical field
 */
void blue_solo_awp(){
  red_solo_awp();
}

// Auton for red left start
void red_left(){
  //7 block long goal
  odom_constants();
  
  chassis.set_coordinates(-49.426, 17.045, 90);
  conveyor.spin(forward, 12, volt);
  conveyor2.spin(forward, 200, rpm);
  scorer.spin(forward, 85, rpm);
  chassis.drive_to_pose(-34.227, 15.612, 90); //ready for 3 stack
  chassis.drive_max_voltage = 4;
  chassis.turn_to_angle(45);
  chassis.drive_to_pose(-17.045, 27.934, 45); //picked up 3 stack
  chassis.turn_to_angle(315);
  chassis.drive_max_voltage = 8;
  chassis.drive_to_pose(-42.561, 48.247, 315); //ready for loader
  chassis.turn_to_angle(270);
  loaderToggle(); //loader down
  sylib::delay(500);
  chassis.drive_distance(14.5); //at loader
  sylib::delay(350);
  chassis.drive_max_voltage = 8;
  chassis.drive_to_pose(-17.561, 48.147, 270); //at long goal
  sylib::delay(300);
  chassis.drive_stop(hold);
  conveyor.spin(forward, 12, volt);
  conveyor2.spin(forward, 200, rpm);
  loaderToggle(); //loader up
  hoodToggle(); //hood up
  scorer.spin(forward, 200, rpm);
  sylib::delay(3000);
  chassis.drive_max_voltage = 12;
  chassis.drive_distance(15);
  hoodToggle(); //hood down
  chassis.drive_distance(-17);
}

// Auton for red right start
void red_right(){
  //4 block long goal, 3 block mid goal split with long goal push.
  //might need to make loader go down to help grab the 3 stack of blocks
  odom_constants();

  chassis.set_coordinates(-46.274, -14.764, 0);
  chassis.drive_max_voltage = 8;
  //parkToggle();

  chassis.drive_to_pose(-46.561, -44.445, 0);
  chassis.turn_to_angle(270);
  loaderToggle();
  sylib::delay(500); 
  chassis.drive_max_voltage = 4;
  conveyor.spin(forward, 12, volt);
  conveyor2.spin(forward, 200, rpm);
  scorer.spin(forward, 85, rpm);
  chassis.drive_distance(14); //at loader
  sylib::delay(350);
  chassis.drive_distance(-33); // at long goal
  conveyor.spin(forward, 12, volt);
  conveyor2.spin(forward, 200, rpm);
  loaderToggle();
  hoodToggle(); //hood up
  scorer.spin(forward, 200, rpm);
  sylib::delay(500); //unloading time
  chassis.drive_distance(10);
  hoodToggle(); // hood down
  conveyor.stop();
  conveyor2.stop();
  scorer.stop();
  chassis.drive_to_pose(-32.233, -33.39, 75); 
  conveyor.spin(forward, 12, volt);
  conveyor2.spin(forward, 200, rpm);
  scorer.spin(forward, 85, rpm);
  chassis.drive_max_voltage = 1.5;
  chassis.drive_to_pose(-13.319, -21.719, 75); //picks up 3 stack
  conveyor.stop();
  conveyor2.stop();
  scorer.stop();
  chassis.drive_to_pose(-7.487, -12.698, 45); // at middle goal
  chassis.drive_distance(2);
  chassis.drive_stop(brake);
  conveyor.spin(reverse, 12, volt);
  conveyor2.spin(reverse, 200, rpm);
  scorer.spin(reverse, 85, rpm);
  sylib::delay(100000); //unloading time
  // conveyor.stop();
  // conveyor2.stop();
  // scorer.stop();
  // chassis.drive_max_voltage = 10;
  // chassis.drive_distance(-33.2);
  // chassis.turn_to_angle(90);
  // chassis.drive_distance(10);
  // chassis.drive_stop(hold);
}

/** 
 * Blue left start
 * Calls the corresponding red side function 
 * b/c of symmetrical field
 */
void blue_left(){
  red_left();
}

/** 
 * Blue right start
 * Calls the corresponding red side function 
 * b/c of symmetrical field
 */
void blue_right(){
  red_right();
}

// Skills auton
void skills(){
  //skills auto, one full long goal with two loaders and a park
  //4 block long goal, 3 block mid goal split with long goal push.
  //might need to make loader go down to help grab the 3 stack of blocks
  odom_constants();
  
  chassis.set_coordinates(-49.426, 17.045, 90);
  conveyor.spin(forward, 12, volt);
  conveyor2.spin(forward, 200, rpm);
  scorer.spin(forward, 85, rpm);
  chassis.drive_to_pose(-34.227, 15.612, 90); //ready for 3 stack
  chassis.drive_max_voltage = 2;
  chassis.turn_to_angle(45);
  chassis.drive_to_pose(-17.045, 27.934, 45); //picked up 3 stack
  chassis.turn_to_angle(315);
  chassis.drive_max_voltage = 8;
  chassis.drive_to_pose(-42.561, 48.247, 315); //ready for loader
  chassis.turn_to_angle(270);
  loaderToggle(); //loader down
  sylib::delay(500);
  chassis.drive_distance(13.5); //at loader
  sylib::delay(3500);
  chassis.drive_to_pose(-17.061, 48.147, 270); //at long goal
  sylib::delay(300);
  chassis.drive_stop(hold);
  conveyor.spin(forward, 12, volt);
  conveyor2.spin(forward, 200, rpm);
  loaderToggle(); //loader up
  hoodToggle(); //hood up
  scorer.spin(forward, 200, rpm);
  sylib::delay(5000); //end of left side
  chassis.drive_distance(15);

  chassis.drive_max_voltage = 8;

  chassis.drive_to_pose(-46.561, -40.445, 0);
  chassis.turn_to_angle(270);
  loaderToggle();
  sylib::delay(500);
  chassis.drive_max_voltage = 4;
  conveyor.spin(forward, 12, volt);
  conveyor2.spin(forward, 200, rpm);
  scorer.spin(forward, 85, rpm);
  chassis.drive_distance(11); //at loader
  sylib::delay(3500);
  chassis.drive_distance(-31); // at long goal
  conveyor.spin(forward, 12, volt);
  conveyor2.spin(forward, 200, rpm);
  loaderToggle();
  hoodToggle(); //hood up
  scorer.spin(forward, 200, rpm);
  sylib::delay(500); //unloading time
  chassis.drive_distance(20);
  hoodToggle(); //hood down
  chassis.drive_max_voltage = 12;
  sylib::delay(800);
  chassis.drive_distance(-20);
  hoodToggle(); //hood up
  chassis.drive_max_voltage = 4;
  chassis.drive_distance(10);
  hoodToggle(); // hood down
  conveyor.stop();
  conveyor2.stop();
  scorer.stop();
  chassis.drive_to_pose(-61.608, -19.062, 0);
  conveyor.spin(forward, 12, volt);
  conveyor2.spin(forward, 200, rpm);
  scorer.spin(forward, 85, rpm);
  chassis.drive_max_voltage = 12;
  chassis.drive_distance(42);
}