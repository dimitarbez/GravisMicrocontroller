
class L298N
{
public:
    L298N(int ena, int in1, int in2, int enb, int in3, int in4);

    void setSpeed(int speed); // changed dutyCycle to speed for clarity
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
    int speed = 120;
};
