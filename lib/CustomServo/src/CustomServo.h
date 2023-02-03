class CustomServo
{
private:
  const int PWMFreq = 50;
  const int PWMResolution = 8;

  int PWMChannel;
  float servoDutyCycle;
  int servoPin;

  float ConvertAngleToDutyCycle(float angle);

public:
  CustomServo();
  ~CustomServo();
  void ChangeServoAngleLinear(float newDutyCycle);
  void ChangeServoAngle(float newServoAngle);
  void Setup(int servoPin, float servoDutyCycle, int pwmChannel);
};
