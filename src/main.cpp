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
#define PINCHER_SERVO_PIN 23
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

L298N rearMotors(32, 33, 25, 14, 26, 27, 0);

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

CustomServo pincherServo;

void controlServos(void *parameter)
{
  while (1)
  {

    String buffer;
    if (xQueueReceive(inputDataQueue, &buffer, (TickType_t)0) == pdPASS)
    {
      Serial.println(buffer);
      const float servoValue = buffer.substring(7).toFloat();
      if (buffer.startsWith("servo1"))
      {
        pincherServo.ChangeServoAngleLinear(servoValue);
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

void configure_uart_via_usb(void)
{
  uart_config_t uart_config = {
      .baud_rate = 115200,
      .data_bits = UART_DATA_8_BITS,
      .parity = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
  };
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
  // pinMode(1, OUTPUT);
  // pinMode(3, OUTPUT);
  
  Serial.begin(115200);

  pincherServo.Setup(0, 50, 0);

  xSerialMutex = xSemaphoreCreateMutex();

  inputDataQueue = xQueueCreate(20, sizeof(char[MAX_QUEUE_ITEM_SIZE]));
  outputDataQueue = xQueueCreate(20, sizeof(char[MAX_QUEUE_ITEM_SIZE]));

  xTaskCreatePinnedToCore(readSerialData, "Read serial", 4096, NULL, 1, NULL, app_cpu);
  // xTaskCreatePinnedToCore(sendSerialData, "Write serial", 4096, NULL, 5, NULL, app_cpu);
  xTaskCreatePinnedToCore(controlServos, "Control servos", 4096, NULL, 1, NULL, app_cpu);
  // xTaskCreatePinnedToCore(controlServos, "Read temperature and humidity", 1024, NULL, 1, NULL, app_cpu);
  pinMode(LED_BUILTIN, OUTPUT);

  rearMotors.setSpeed(150);
  rearMotors.moveForward();
  delay(2000);
  rearMotors.moveBackward();
  delay(2000);
  rearMotors.stopMotors();  

}

void loop()
{
}