#include <Arduino.h>
#include <Servo.h>

class CustomServo
{
private:
  Servo servo; // Servo instance to control the servo motor
  float servoAngle; // Current angle of the servo
  int servoPin; // Pin number to which the servo is attachedprivate:
  static const int SERVO_STEP_DELAY_MS = 15;

  int ConvertAngleToPWM(float angle); // Convert a servo angle to PWM value

public:
  CustomServo(); // Constructor
  ~CustomServo(); // Destructor

  void Setup(int servoPin, float angle); // Set up the servo motor
  void ChangeServoAngleLinear(float newServoAngle); // Change the servo angle with a linear motion
  void ChangeServoAngle(float newServoAngle); // Immediately set the servo to a given angle
};
