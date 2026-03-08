#include "vex.h"
#include "robot-config.h"
#include <cmath>
#include <utility>

using namespace vex;

// Tuning constants
const double curvatureFactor = 1.0;   // Higher = more turn response
const double accelRate = 0.1;         // Smoothing (0–1), smaller = smoother

// Previous smoothed speeds
double lastLeft = 0, lastRight = 0;

void curvatureDrive(double forward, double curvature) {
    double leftPower, rightPower;

    // Curvature affects how sharply it turns
    leftPower = forward + fabs(forward) * curvature * curvatureFactor;
    rightPower = forward - fabs(forward) * curvature * curvatureFactor;

    // Normalize values
    double maxMag = fmax(fabs(leftPower), fabs(rightPower));
    if (maxMag > 1.0) {
    leftPower /= maxMag;
    rightPower /= maxMag;
    }

    // Apply smoothing
    leftPower = lastLeft + (leftPower - lastLeft) * accelRate;
    rightPower = lastRight + (rightPower - lastRight) * accelRate;
    lastLeft = leftPower;
    lastRight = rightPower;

    // Send to motors with voltage control
    double leftVolt = leftPower * 12.0;
    double rightVolt = rightPower * 12.0;

    leftFront.spin(fwd, leftVolt, voltageUnits::volt);
    leftMiddle.spin(fwd, leftVolt, voltageUnits::volt);
    leftBack.spin(fwd, leftVolt, voltageUnits::volt);
    rightFront.spin(fwd, rightVolt, voltageUnits::volt);
    rightMiddle.spin(fwd, rightVolt, voltageUnits::volt);
    rightBack.spin(fwd, rightVolt, voltageUnits::volt);
}

void updateCurvatureDrive() {
  double forward = Controller.Axis3.position(percent) / 100.0;
  double curvature = Controller.Axis1.position(percent) / 100.0;
  curvatureDrive(forward, curvature);
}

void cubic() {
    // x₁ = Left Motors X = [Controller] [3] position + [Controller] [1] position
    // x₂ = Right Motors X = [Controller] [3] position - [Controller] [1] position
    // Sets each motor's velocity (in rpm) to 0.0001x³ (shown as 0.0001 * x * x * x)
    leftFront.spin(forward, (0.0001 * (((Controller.Axis3.position() + Controller.Axis1.position()) * (Controller.Axis3.position() + Controller.Axis1.position())) * (Controller.Axis3.position() + Controller.Axis1.position()))), percent);
    leftMiddle.spin(forward, (0.0001 * (((Controller.Axis3.position() + Controller.Axis1.position()) * (Controller.Axis3.position() + Controller.Axis1.position())) * (Controller.Axis3.position() + Controller.Axis1.position()))), percent);
    leftBack.spin(forward, (0.0001 * (((Controller.Axis3.position() + Controller.Axis1.position()) * (Controller.Axis3.position() + Controller.Axis1.position())) * (Controller.Axis3.position() + Controller.Axis1.position()))), percent);
    rightFront.spin(forward, (0.0001 * (((Controller.Axis3.position() - Controller.Axis1.position()) * (Controller.Axis3.position() - Controller.Axis1.position())) * (Controller.Axis3.position() - Controller.Axis1.position()))), percent);
    rightMiddle.spin(forward, (0.0001 * (((Controller.Axis3.position() - Controller.Axis1.position()) * (Controller.Axis3.position() - Controller.Axis1.position())) * (Controller.Axis3.position() - Controller.Axis1.position()))), percent);
    rightBack.spin(forward, (0.0001 * (((Controller.Axis3.position() - Controller.Axis1.position()) * (Controller.Axis3.position() - Controller.Axis1.position())) * (Controller.Axis3.position() - Controller.Axis1.position()))), percent);
}

void quadratic() {
    // Calculate raw inputs for Left and Right sides
    double leftInput = Controller.Axis3.position() + Controller.Axis1.position();
    double rightInput = Controller.Axis3.position() - Controller.Axis1.position();

    // Apply the formula: 0.01 * x * abs(x)
    // Using abs() ensures the sign (direction) is preserved
    double leftOutput = 0.01 * leftInput * std::abs(leftInput);
    double rightOutput = 0.01 * rightInput * std::abs(rightInput);

    // Spin Motors
    leftFront.spin(forward, leftOutput, percent);
    leftMiddle.spin(forward, leftOutput, percent);
    leftBack.spin(forward, leftOutput, percent);

    rightFront.spin(forward, rightOutput, percent);
    rightMiddle.spin(forward, rightOutput, percent);
    rightBack.spin(forward, rightOutput, percent);
}