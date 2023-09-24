#include "CustomServo.h"
#include <Arduino.h>

CustomServo::CustomServo()
{
}

CustomServo::~CustomServo()
{
}

void CustomServo::Setup(int servoPin, float angle)
{
  this->servoPin = servoPin;
  servo.attach(servoPin);
  ChangeServoAngle(angle);
}

void CustomServo::ChangeServoAngleLinear(float newServoAngle)
{
  const int currentAngle = ConvertAngleToPWM(this->servoAngle);
  const int targetAngle = ConvertAngleToPWM(newServoAngle);
  int step = currentAngle < targetAngle ? 1 : -1;

  for (int angle = currentAngle; angle != targetAngle; angle += step)
  {
    servo.write(angle);
    delay(15); // delay to give servo some time to move. Adjust as necessary.
  }

  this->servoAngle = newServoAngle;
}

void CustomServo::ChangeServoAngle(float newServoAngle)
{
  servo.write(ConvertAngleToPWM(newServoAngle));
  this->servoAngle = newServoAngle;
}

int CustomServo::ConvertAngleToPWM(float angle)
{
  return constrain(angle, 0, 180); // constrain the angle between 0 and 180 degrees.
}
