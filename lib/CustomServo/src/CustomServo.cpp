#include "CustomServo.h"
#include <Arduino.h>

CustomServo::CustomServo()
{
}

CustomServo::~CustomServo()
{
}

void CustomServo::Setup(int servoPin, float servoDutyCycle, int pwmChannel)
{
  this->servoDutyCycle = servoDutyCycle;
  this->PWMChannel = pwmChannel;
  this->servoPin = servoPin;
  ledcSetup(pwmChannel, this->PWMFreq, this->PWMResolution);
  ledcAttachPin(servoPin, pwmChannel);
  ledcWrite(pwmChannel, servoDutyCycle);
}

void CustomServo::ChangeDutyCycleLinear(float newDutyCycle)
{
  if (this->servoDutyCycle < newDutyCycle)
  {
    for (float i = this->servoDutyCycle; i < newDutyCycle; i += 0.001)
    {
      ledcWrite(this->PWMChannel, i);
      this->servoDutyCycle = i;
    }
  }
  else if (this->servoDutyCycle > newDutyCycle)
  {
    for (float i = this->servoDutyCycle; i > newDutyCycle; i -= 0.001)
    {
      ledcWrite(this->PWMChannel, i);
      this->servoDutyCycle = i;
    }
  }
}
