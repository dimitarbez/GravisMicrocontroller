class CustomServo
{
private:
  const int PWMFreq = 50;
  const int PWMResolution = 8;

  int PWMChannel;
  float servoDutyCycle;
  int servoPin;

public:
  CustomServo();
  ~CustomServo();
  void ChangeDutyCycleLinear(float newDutyCycle);
  void Setup(int servoPin, float servoDutyCycle, int pwmChannel);
};
