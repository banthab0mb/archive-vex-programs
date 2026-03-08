#pragma once
#include "JAR-Template/drive.h"

class Drive;

extern Drive chassis;

void default_constants();

// Control functions
void loaderToggle();
void parkToggle();
void trapdoorToggle();
void wingsToggle();

// Autons

void drive_forward();
void red_solo_awp();
void blue_solo_awp();
void red_left();
void red_right();
void blue_left();
void blue_right();
void skills();