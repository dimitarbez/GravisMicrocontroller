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
  const float oldDutyCycle = this->servoDutyCycle;
  const float dutyCycleIncrement = 0.01;

  if (this->servoDutyCycle < newDutyCycle)
  {
    while (this->servoDutyCycle <= newDutyCycle)
    {
      this->servoDutyCycle += dutyCycleIncrement;
      ledcWrite(this->PWMChannel, servoDutyCycle);
      vTaskDelay(20 / portTICK_PERIOD_MS);
    }
  }
  else
  {
    while (this->servoDutyCycle >= newDutyCycle)
    {
      this->servoDutyCycle -= dutyCycleIncrement;
      ledcWrite(this->PWMChannel, servoDutyCycle);
      vTaskDelay(20 / portTICK_PERIOD_MS);
    }
  }
}
