#include <Arduino.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <CustomServo.h>
#include <L298N_ArduinoMega.h>
#include <Adafruit_NeoPixel.h>

namespace Pins
{
  constexpr int LED_STRIP_FRONT = 8;
  constexpr int LED_STRIP_BACK = 7;
  constexpr int DHTPIN = 25;

  constexpr int REAR_ENA = 29;
  constexpr int REAR_IN1 = 31;
  constexpr int REAR_IN2 = 33;
  constexpr int REAR_IN3 = 35;
  constexpr int REAR_IN4 = 37;
  constexpr int REAR_ENB = 39;

  constexpr int FRONT_ENA = 28;
  constexpr int FRONT_IN1 = 30;
  constexpr int FRONT_IN2 = 32;
  constexpr int FRONT_IN3 = 34;
  constexpr int FRONT_IN4 = 36;
  constexpr int FRONT_ENB = 38;

  constexpr int CAM_YAW_SERVO = 2;
  constexpr int CAM_PITCH_SERVO = 3;
}

L298N rearMotors(Pins::REAR_ENA, Pins::REAR_IN1, Pins::REAR_IN2, Pins::REAR_ENB, Pins::REAR_IN3, Pins::REAR_IN4);
L298N frontMotors(Pins::FRONT_ENA, Pins::FRONT_IN1, Pins::FRONT_IN2, Pins::FRONT_ENB, Pins::FRONT_IN3, Pins::FRONT_IN4);

CustomServo camPitchServo, camYawServo;

Adafruit_NeoPixel frontStrip = Adafruit_NeoPixel(20, Pins::LED_STRIP_FRONT, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel backStrip = Adafruit_NeoPixel(20, Pins::LED_STRIP_BACK, NEO_GRB + NEO_KHZ800);

void servoControl(String buffer)
{
  int separatorIndex = buffer.indexOf(":");
  String command = buffer.substring(separatorIndex + 1);

  separatorIndex = command.indexOf(":");
  float angle = command.substring(separatorIndex + 1).toFloat();
  command = command.substring(0, separatorIndex);

  Serial.println(command);
  Serial.println(angle);

  if (command == "campitch")
  {
    Serial.println("ACK SERVO");
    camPitchServo.ChangeServoAngleLinear(angle);
  }
  else if (command == "camyaw")
  {
    Serial.println("ACK SERVO");
    camYawServo.ChangeServoAngleLinear(angle);
  }
}

void motorControl(String buffer)
{
  int separatorIndex = buffer.indexOf(":");
  String command = buffer.substring(separatorIndex + 1);
  Serial.println(command);
  if (command == "forward")
  {
    Serial.println("ACK MOTOR");
    rearMotors.moveForward();
    frontMotors.moveForward();
  }
  else if (command == "backward")
  {
    Serial.println("ACK MOTOR");
    frontMotors.moveBackward();
    rearMotors.moveBackward();
  }
  else if (command == "left")
  {
    Serial.println("ACK MOTOR");
    rearMotors.moveLeft();
    frontMotors.moveLeft();
  }
  else if (command == "right")
  {
    Serial.println("ACK MOTOR");
    rearMotors.moveRight();
    frontMotors.moveRight();
  }
  else if (command == "stop")
  {
    Serial.println("ACK MOTOR");
    rearMotors.stopMotors();
    frontMotors.stopMotors();
  }
  else if (command.startsWith("speed"))
  {
    int separatorIndex = command.indexOf(":");
    int speed = command.substring(separatorIndex + 1).toInt();
    Serial.println("ACK MOTOR");
    rearMotors.setSpeed(speed);
    frontMotors.setSpeed(speed);
  }
}

void setFrontStripColor(int r, int g, int b)
{
  for (size_t i = 0; i < frontStrip.numPixels(); i++)
  {
    frontStrip.setPixelColor(i, r, g, b);
  }
  frontStrip.setBrightness(255);
  frontStrip.show();
}

void setBackStripColor(int r, int g, int b)
{
  for (size_t i = 0; i < frontStrip.numPixels(); i++)
  {
    backStrip.setPixelColor(i, r, g, b);
  }
  backStrip.setBrightness(255);
  backStrip.show();
}

void parseRGB(String &input, int &r, int &g, int &b)
{
  r = input.substring(0, input.indexOf(":")).toInt();
  input.remove(0, input.indexOf(":") + 1);

  g = input.substring(0, input.indexOf(":")).toInt();
  input.remove(0, input.indexOf(":") + 1);

  b = input.toInt();
}

void lightControl(String buffer)
{
  int separatorIndex = buffer.indexOf(":");
  String command = buffer.substring(separatorIndex + 1);

  if (command.startsWith("front"))
  {
    command.remove(0, command.indexOf(":") + 1);
    int r, g, b;
    parseRGB(command, r, g, b);
    setFrontStripColor(r, g, b);
  }
  else if (command.startsWith("back"))
  {
    command.remove(0, command.indexOf(":") + 1);
    int r, g, b;
    parseRGB(command, r, g, b);
    setBackStripColor(r, g, b);
  }
  else if (command == "off")
  {
    setFrontStripColor(0, 0, 0);
    setBackStripColor(0, 0, 0);
    frontStrip.setBrightness(0);
    backStrip.setBrightness(0);
    frontStrip.show();
    backStrip.show();
  }
}

void setup()
{
  for (int i = 0; i < 5; i++)
  {                                  // Blinking 10 times per second for 3 seconds
    digitalWrite(LED_BUILTIN, HIGH); // Turn the LED on
    delay(500);                       // Wait for 50ms
    digitalWrite(LED_BUILTIN, LOW);  // Turn the LED off
    delay(500);                       // Wait for 50ms
  }

  Serial.begin(115200);

  delay(2000);

  camPitchServo.Setup(Pins::CAM_PITCH_SERVO, 90.0);
  camYawServo.Setup(Pins::CAM_YAW_SERVO, 90.0);

  frontStrip.begin();
  backStrip.begin();

  frontStrip.show();
  backStrip.show();
}

void loop()
{
  if (Serial.available() > 0)
  {
    String buffer = Serial.readStringUntil('\n');
    if (buffer.startsWith("motor"))
    {
      motorControl(buffer);
    }
    else if (buffer.startsWith("servo"))
    {
      servoControl(buffer);
    }
    else if (buffer.startsWith("lights"))
    {
      lightControl(buffer);
    }
  }
}
