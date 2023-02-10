#include <Arduino.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <CustomServo.h>
#include "driver/uart.h"
#include "driver/gpio.h"
#include <L298N_ESP32.h>
#include <Adafruit_MPU6050.h>

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
Adafruit_MPU6050 mpu;
sensors_event_t a, g, temp;

L298N rearMotors(REAR_ENA, REAR_IN1, REAR_IN2, REAR_ENB, REAR_IN3, REAR_IN4, 0);
L298N frontMotors(FRONT_ENA, FRONT_IN1, FRONT_IN2, FRONT_ENB, FRONT_IN3, FRONT_IN4, 0);

SemaphoreHandle_t xSerialMutex;
QueueHandle_t inputDataQueue;
QueueHandle_t outputDataQueue;

CustomServo pincherServo;

void readIMUData(void *parameter)
{
  mpu.getEvent(&a, &g, &temp);
  Serial.print("accel/x:");
  Serial.print(a.acceleration.x);
  Serial.print("/y:");
  Serial.print(a.acceleration.y);
  Serial.print("/z:");
  Serial.println(a.acceleration.z);

  Serial.print("rotat/x:");
  Serial.print(g.gyro.x);
  Serial.print("/y:");
  Serial.print(g.gyro.y);
  Serial.print("/z:");
  Serial.println(g.gyro.z);

  Serial.print("temperature:");
  Serial.print(temp.temperature);

  Serial.println("");
}

void setup()
{
  Serial.begin(115200);
  if (!mpu.begin())
  {
    Serial.println("Sensor init failed");
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_260_HZ);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  delay(2000);
  display.clearDisplay();
  display.display();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setRotation(2);
  display.setCursor(0, 16);
  display.print("Gravis");
  display.display();
  display.startscrollright(0x00, 0x07);
}

void motorControl(String buffer)
{
  Serial.println("ACK MOTOR");
  int separatorIndex = buffer.indexOf(":");
  String command = buffer.substring(separatorIndex + 1);
  Serial.println(command);
  if (command == "forward")
  {
    Serial.println("RUN");
    rearMotors.moveForward();
    frontMotors.moveForward();
  }
  else if (command == "backward")
  {
    rearMotors.moveBackward();
    frontMotors.moveBackward();
  }
  else if (command == "left")
  {
    rearMotors.moveLeft();
    frontMotors.moveLeft();
  }
  else if (command == "right")
  {
    rearMotors.moveRight();
    frontMotors.moveRight();
  }
  else if (command == "stop")
  {
    rearMotors.stopMotors();
    frontMotors.stopMotors();
  }
  else if (command.startsWith("speed"))
  {
    int separatorIndex = command.indexOf(":");
    int speed = command.substring(separatorIndex + 1).toInt();
    Serial.println(speed);
    rearMotors.setSpeed(speed);
    frontMotors.setSpeed(speed);
  }
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
  }
  readIMUData(NULL);
}