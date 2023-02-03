#include <Arduino.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#if CONFIG_FREE_RTOS_UNICORE
static const BaseType_t app_cpu = 0;

#else
static const BaseType_t app_cpu = 1;
#endif
#define DHTPIN 36 // Pin where DHT22 is connected
#define PITCHER_SERVO_PIN 23
#define ARMYAW_SERVO_PIN 1
#define ARMEXTENSION_SERVO_PIN 3
#define ARMHEIGHT_SERVO_PIN 0
#define CAMERAYAW_SERVO_PIN 2

#define DHTTYPE DHT22 // Type of DHT22 sensor

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define MAX_QUEUE_ITEM_SIZE 50

DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

/* Setting PWM properties */
const int PWMFreq = 50;
const int PWMChannelPincher = 0;
const int PWMChannelArmYaw = 1;


const int PWMResolution = 8;

int servoPincherDutyCycle = 0;
int armYawDutyCycle = 0;

SemaphoreHandle_t xSerialMutex;
QueueHandle_t inputDataQueue;
QueueHandle_t outputDataQueue;

void controlServos(void *parameter)
{
  while (1)
  {

    String buffer;
    if (xQueueReceive(inputDataQueue, &buffer, (TickType_t)0) == pdPASS)
    {
      Serial.println(buffer);
      if(buffer.startsWith("servo1")) {
        Serial.println(buffer.substring(7).toInt());
        ledcWrite(PWMChannelPincher, buffer.substring(7).toInt());
      } else if (buffer.startsWith("servo2")) {
        ledcWrite(PWMChannelArmYaw, buffer.substring(7).toInt());
      }
    }
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

// size_t delimeterPos = buffer.find(':');
// if (buffer.substr(0, delimeterPos - 1) == "servo")
// {

//   xQueueReceive(inputDataQueue, NULL, 10);
//   std::string servoName = buffer.substr(0, delimeterPos);
//   int servoValue = std::stoi(buffer.substr(delimeterPos + 1, 3));

//   // check for servo1 command
//   if (servoName == "servo1")
//   {
//     pincherServo.write(servoValue);
//   }
//   // check for servo2 command
//   else if (servoName == "servo2")
//   {
//     armYawServo.write(servoValue);
//   }
//   // check for servo3 command
//   else if (servoName == "servo3")
//   {
//     armExtensionServo.write(servoValue);
//   }
//   // check for servo4 command
//   else if (servoName == "servo4")
//   {
//     armHeightServo.write(servoValue);
//   }
//   // check for servo5 command
//   else if (servoName == "servo5")
//   {
//     cameraYawServo.write(servoValue);
//   }

// void readHumidity(void *parameter)
// {
//   while (1)
//   {
//     float humidityReading = dht.readHumidity();
//     float temperatureReading = dht.readTemperature();

//     String humidityData = "humid:";
//     humidityData.concat(humidityReading);

//     String tempData = "temp:";
//     tempData.concat(temperatureReading);

//     xQueueSend(outputDataQueue, (void *)&humidityData, 10);
//     xQueueSend(outputDataQueue, (void *)&tempData, 10);
//   }
// }

// void writeToOLED(void *parameter)
// {
//   while (1)
//   {
//     display.clearDisplay();
//     display.setTextSize(1);
//     display.setCursor(0, 10);
//     display.setTextColor(WHITE);
//     display.println("Booting Gravis...");
//     display.display();
//   }
// }

void readSerialData(void *parameter)
{
  while (1)
  {

    if (Serial.available() > 0)
    {

      String item = Serial.readStringUntil('\n');
      if (xQueueSend(inputDataQueue, (void *)&item, (TickType_t)10) == pdPASS)
      {
        Serial.print("ACK ");
        Serial.println(item);
      }
      else
      {
        Serial.println("Error sending in queue");
      }
    }
  }
}

// void sendSerialData(void *parameter)
// {
//   while (1)
//   {
//     String buffer;
//     if (xQueueReceive(outputDataQueue, (void *)&buffer, (TickType_t)10) == pdTRUE)
//     {
//       if (xSemaphoreTake(xSerialMutex, (TickType_t)10) == pdTRUE)
//       {
//         Serial.println(buffer);
//         xSemaphoreGive(xSerialMutex);
//       }
//     }
//   }
// }

void setup()
{
  Serial.begin(115200);
  ledcSetup(PWMChannelPincher, PWMFreq, PWMResolution);
  ledcAttachPin(PITCHER_SERVO_PIN, PWMChannelPincher);

  ledcSetup(PWMChannelArmYaw, PWMFreq, PWMResolution);
  ledcAttachPin(0, PWMChannelArmYaw);

  ledcWrite(PWMChannelPincher, servoPincherDutyCycle);
  ledcWrite(PWMChannelArmYaw, armYawDutyCycle);
  
  dht.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { // Address 0x3D for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  delay(500);
  display.clearDisplay();
  display.setRotation(180);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println("Booting Gravis...");
  display.display();

  xSerialMutex = xSemaphoreCreateMutex();

  inputDataQueue = xQueueCreate(20, sizeof(char[MAX_QUEUE_ITEM_SIZE]));
  outputDataQueue = xQueueCreate(20, sizeof(char[MAX_QUEUE_ITEM_SIZE]));

  xTaskCreatePinnedToCore(readSerialData, "Read serial", 4096, NULL, 1, NULL, app_cpu);
  //xTaskCreatePinnedToCore(sendSerialData, "Write serial", 4096, NULL, 5, NULL, app_cpu);
  xTaskCreatePinnedToCore(controlServos, "Control servos", 4096, NULL, 1, NULL, app_cpu);
  //xTaskCreatePinnedToCore(controlServos, "Read temperature and humidity", 1024, NULL, 1, NULL, app_cpu);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
}