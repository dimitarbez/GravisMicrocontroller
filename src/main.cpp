#include <Arduino.h>
#include <Servo.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DHTPIN 36 // Pin where DHT22 is connected
#define PITCHER_SERVO_PIN 23
#define ARMYAW_SERVO_PIN 1
#define ARMEXTENSION_SERVO_PIN 3
#define ARMHEIGHT_SERVO_PIN 0
#define CAMERAYAW_SERVO_PIN 2

#define DHTTYPE DHT22 // Type of DHT22 sensor

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

Servo pincherServo, armYawServo, armExtensionServo, armHeightServo, cameraYawServo;

void setup()
{
  Serial.begin(115200);
  pincherServo.attach(PITCHER_SERVO_PIN);
  armYawServo.attach(ARMYAW_SERVO_PIN);
  armExtensionServo.attach(ARMEXTENSION_SERVO_PIN);
  armHeightServo.attach(ARMHEIGHT_SERVO_PIN);
  cameraYawServo.attach(CAMERAYAW_SERVO_PIN);
  dht.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { // Address 0x3D for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  delay(500);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println("Booting Gravis...");
  display.display();
}

void loop()
{

  float humidityReading = dht.readHumidity();
  float temperatureReading = dht.readTemperature();

  if (Serial.available() > 0)
  {
    String command = Serial.readStringUntil('\n');
    int servoNumber, servoValue;
    // check for servo1 command
    if (command.startsWith("servo1:"))
    {
      servoValue = command.substring(7).toInt();
      pincherServo.write(servoValue);
    }
    // check for servo2 command
    else if (command.startsWith("servo2:"))
    {
      servoValue = command.substring(7).toInt();
      armYawServo.write(servoValue);
    }
    // check for servo3 command
    else if (command.startsWith("servo3:"))
    {
      servoValue = command.substring(7).toInt();
      armExtensionServo.write(servoValue);
    }
    // check for servo4 command
    else if (command.startsWith("servo4:"))
    {
      servoValue = command.substring(7).toInt();
      armHeightServo.write(servoValue);
    }
    // check for servo5 command
    else if (command.startsWith("servo5:"))
    {
      servoValue = command.substring(7).toInt();
      cameraYawServo.write(servoValue);
    }
  }
}