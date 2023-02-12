#include <Arduino.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <CustomServo.h>
#include "driver/uart.h"
#include "driver/gpio.h"
#include <L298N_ESP32.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define LED_STRIP_FRONT_PIN 13
#define LED_STRIP_BACK_PIN 12

#define DHTPIN 36 // Pin where DHT22 is connected

#define DHTTYPE DHT22 // Type of DHT22 sensor

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define REAR_ENA 32
#define REAR_IN1 33
#define REAR_IN2 25
#define REAR_IN3 26
#define REAR_IN4 27
#define REAR_ENB 14

#define FRONT_ENA 19
#define FRONT_IN1 5
#define FRONT_IN2 18
#define FRONT_IN3 16
#define FRONT_IN4 17
#define FRONT_ENB 4

// DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

L298N rearMotors(REAR_ENA, REAR_IN1, REAR_IN2, REAR_ENB, REAR_IN3, REAR_IN4, 0);
L298N frontMotors(FRONT_ENA, FRONT_IN1, FRONT_IN2, FRONT_ENB, FRONT_IN3, FRONT_IN4, 0);

SemaphoreHandle_t xSerialMutex;
QueueHandle_t inputDataQueue;
QueueHandle_t outputDataQueue;

CustomServo servo1, servo2, servo3, servo4;

Adafruit_NeoPixel frontStrip = Adafruit_NeoPixel(20, LED_STRIP_FRONT_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel backStrip = Adafruit_NeoPixel(20, LED_STRIP_BACK_PIN, NEO_GRB + NEO_KHZ800);

void servoControl(String buffer)
{
  int separatorIndex = buffer.indexOf(":");
  String command = buffer.substring(separatorIndex + 1);

  separatorIndex = command.indexOf(":");
  float angle = command.substring(separatorIndex + 1).toFloat();
  command = command.substring(0, separatorIndex);

  Serial.println(command);
  Serial.println(angle);

  if (command == "pincher")
  {
    Serial.println("ACK SERVO");
    servo1.ChangeServoAngleLinear(angle);
  }
  else if (command == "armyaw")
  {
    Serial.println("ACK SERVO");
    servo4.ChangeServoAngleLinear(angle);
  }
  else if (command == "armext")
  {
    Serial.println("ACK SERVO");
    servo3.ChangeServoAngleLinear(angle);
  }
  else if (command == "armheight")
  {
    Serial.println("ACK SERVO");
    servo2.ChangeServoAngleLinear(angle);
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

void lightControl(String buffer)
{
  int separatorIndex = buffer.indexOf(":");
  String command = buffer.substring(separatorIndex + 1);
  if (command.startsWith("front"))
  {
    separatorIndex = command.indexOf(":");
    int r = command.substring(separatorIndex + 1).toInt();
    command = command.substring(separatorIndex + 1);

    separatorIndex = command.indexOf(":");
    int g = command.substring(separatorIndex + 1).toInt();
    command = command.substring(separatorIndex + 1);

    separatorIndex = command.indexOf(":");
    int b = command.substring(separatorIndex + 1).toInt();

    setFrontStripColor(r, g, b);
  }
  else if (command.startsWith("back"))
  {
    separatorIndex = command.indexOf(":");
    int r = command.substring(separatorIndex + 1).toInt();
    command = command.substring(separatorIndex + 1);

    separatorIndex = command.indexOf(":");
    int g = command.substring(separatorIndex + 1).toInt();
    command = command.substring(separatorIndex + 1);

    separatorIndex = command.indexOf(":");
    int b = command.substring(separatorIndex + 1).toInt();

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

void oledControl(String buffer)
{
  int separatorIndex = buffer.indexOf(":");
  int secondSeparatorIndex = buffer.indexOf(":", separatorIndex + 1);
  int size = buffer.substring(separatorIndex + 1, secondSeparatorIndex).toInt();
  String text = buffer.substring(secondSeparatorIndex + 1);

  display.clearDisplay();
  display.setTextSize(size);
  display.setTextColor(WHITE);
  display.setRotation(2);
  display.setCursor(0, 8);
  display.print(text);
  display.display();
}

void setup()
{
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  delay(2000);
  display.clearDisplay();
  // display.display();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setRotation(2);
  display.setCursor(0, 8);
  display.print("Gravis");
  display.display();
  display.startscrollright(0x00, 0x07);

  servo1.Setup(23, 90, 1);
  servo2.Setup(0, 90, 2);
  servo3.Setup(2, 90, 3);
  servo4.Setup(15, 90, 4);

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
    else if (buffer.startsWith("oled")) {
      oledControl(buffer);
    }
  }
}
