#include <driver/gpio.h>
#include <Arduino.h>

class L298N
{
public:
    L298N(int ena, int in1, int in2, int enb, int in3, int in4, int pwmChannel);

    void setSpeed(int dutyCycle);
    void moveForward();
    void moveBackward();
    void moveLeft();
    void moveRight();
    void stopMotors();

private:
    int _ena;
    int _in1;
    int _in2;
    int _enb;
    int _in3;
    int _in4;
    // Setting PWM properties
    int freq = 1000;
    int pwmChannel = 0;
    const int resolution = 8;
    int dutyCycle = 120;
};