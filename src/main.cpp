#include <Arduino.h>
#include <Servo.h>
#include <DHT.h>

#define DHTPIN 2      // Pin where DHT22 is connected
#define DHTTYPE DHT22 // Type of DHT22 sensor

DHT dht(DHTPIN, DHTTYPE);
Servo pincherServo, armYawServo, armExtensionServo, armHeightServo, cameraYawServo;

void setup()
{
  Serial.begin(9600);
  pincherServo.attach(12);
  armYawServo.attach(13);
  armExtensionServo.attach(14);
  armHeightServo.attach(15);
  cameraYawServo.attach(16);
  dht.begin();
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