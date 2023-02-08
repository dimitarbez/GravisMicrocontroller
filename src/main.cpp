#include <Arduino.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <CustomServo.h>
#include "driver/uart.h"
#include "driver/gpio.h"
#include <L298N_ESP32.h>

#if CONFIG_FREE_RTOS_UNICORE
static const BaseType_t app_cpu = 0;

#else
static const BaseType_t app_cpu = 1;
#endif
#define DHTPIN 36 // Pin where DHT22 is connected

#define DHTTYPE DHT22 // Type of DHT22 sensor

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define MAX_QUEUE_ITEM_SIZE 50

#define REAR_ENA 32
#define REAR_ENB 14
#define REAR_IN1 33
#define REAR_IN2 25
#define REAR_IN3 26
#define REAR_IN4 27

#define FRONT_ENA 19
#define FRONT_ENB 4
#define FRONT_IN1 18
#define FRONT_IN2 5
#define FRONT_IN3 17
#define FRONT_IN4 16

DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

L298N rearMotors(REAR_ENA, REAR_IN1, REAR_IN2, REAR_ENB, REAR_IN3, REAR_IN4, 0);
L298N frontMotors(FRONT_ENA, FRONT_IN1, FRONT_IN2, FRONT_ENB, FRONT_IN3, FRONT_IN4, 0);

SemaphoreHandle_t xSerialMutex;
QueueHandle_t inputDataQueue;
QueueHandle_t outputDataQueue;

CustomServo pincherServo;

// void controlServos(void *parameter)
// {
//   while (1)
//   {

//     String buffer;
//     if (xQueueReceive(inputDataQueue, &buffer, (TickType_t)0) == pdPASS)
//     {
//       Serial.println(buffer);
//       const float servoValue = buffer.substring(7).toFloat();
//       if (buffer.startsWith("servo1"))
//       {
//         pincherServo.ChangeServoAngleLinear(servoValue);
//       }
//     }
//   }
// }

void readDHTData(void *parameter)
{
  while (1)
  {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    // Reading temperature or humidity takes about 250 milliseconds!
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t))
    {
      continue;
    }

    String humidity = "humidity:";
    humidity = humidity.concat(h);

    String temperature = "temperature:";
    temperature = temperature.concat(t);

    xQueueSend(outputDataQueue, (void *)&temperature, 10);
    xQueueSend(outputDataQueue, (void *)&humidity, 0);
  }
}

void controlMotors(void *parameter)
{
  while (1)
  {
    String buffer;
    if (xQueuePeek(inputDataQueue, &buffer, (TickType_t)10) == pdTRUE)
    {
      Serial.println(buffer);
      if (buffer.startsWith("motor"))
      {
        if (xQueueReceive(inputDataQueue, &buffer, (TickType_t)10) == pdTRUE)
        {
          int separatorIndex = buffer.indexOf(":");
          String command = buffer.substring(separatorIndex + 1);
          if (command == "forward")
          {
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
            rearMotors.setSpeed(speed);
            frontMotors.setSpeed(speed);
          }
        }
      }
    }
  }
}

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

void writeSerialData(void *parameter)
{
  while (1)
  {
    String buffer;
    if (xQueueReceive(outputDataQueue, &buffer, 10) == pdTRUE)
    {
      Serial.println(buffer);
    }
  }
}

void configure_uart_via_usb(void)
{
  uart_config_t uart_config = {
      .baud_rate = 115200,
      .data_bits = UART_DATA_8_BITS,
      .parity = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};
  uart_param_config(UART_NUM_0, &uart_config);
  uart_driver_install(UART_NUM_0, 2048, 2048, 0, NULL, 0);

  gpio_pad_select_gpio(GPIO_NUM_1);
  gpio_pad_select_gpio(GPIO_NUM_3);
  gpio_set_direction(GPIO_NUM_1, GPIO_MODE_OUTPUT);
  gpio_set_direction(GPIO_NUM_3, GPIO_MODE_OUTPUT);
}

void setup()
{
  void configure_uart_via_usb(void);

  Serial.begin(115200);

  xSerialMutex = xSemaphoreCreateMutex();

  inputDataQueue = xQueueCreate(20, sizeof(char[MAX_QUEUE_ITEM_SIZE]));
  outputDataQueue = xQueueCreate(20, sizeof(char[MAX_QUEUE_ITEM_SIZE]));

  xTaskCreatePinnedToCore(readSerialData, "Read serial", 4096, NULL, 1, NULL, app_cpu);
  xTaskCreatePinnedToCore(writeSerialData, "Write serial", 4096, NULL, 1, NULL, app_cpu);

  xTaskCreatePinnedToCore(controlMotors, "Control motors", 4096, NULL, 1, NULL, app_cpu);
}

void loop()
{
}