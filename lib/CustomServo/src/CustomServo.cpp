#include "CustomServo.h"
#include <Arduino.h>

CustomServo::CustomServo() : servoPin(-1), servoAngle(0.0)
{
}

CustomServo::~CustomServo()
{
  if (servo.attached())
  {
    servo.detach();
  }
}

void CustomServo::Setup(int servoPin, float angle)
{
  this->servoPin = servoPin;
  ChangeServoAngle(angle);
}

void CustomServo::ChangeServoAngleLinear(float newServoAngle)
{
  if (!servo.attached())
  {
    servo.attach(servoPin);
  }

  const int currentAngle = ConvertAngleToPWM(this->servoAngle);
  const int targetAngle = ConvertAngleToPWM(newServoAngle);
  int step = currentAngle < targetAngle ? 1 : -1;

  for (int angle = currentAngle; angle != targetAngle; angle += step)
  {
    servo.write(angle);
    delay(15); // delay to give servo some time to move. Adjust as necessary.
  }

  this->servoAngle = newServoAngle;
  servo.detach(); // detach after operation
}

void CustomServo::ChangeServoAngle(float newServoAngle)
{
  if (!servo.attached())
  {
    servo.attach(servoPin);
  }

  servo.write(ConvertAngleToPWM(newServoAngle));
  this->servoAngle = newServoAngle;
  servo.detach(); // detach after operation
}

int CustomServo::ConvertAngleToPWM(float angle)
{
  return constrain(angle, 0, 180); // constrain the angle between 0 and 180 degrees.
}
