#include "Lighting.h"

Lighting::Lighting(int frontStripPin, int backStripPin, int numOfPixels)
    : frontStrip(numOfPixels, frontStripPin, NEO_GRB + NEO_KHZ800),
      backStrip(numOfPixels, backStripPin, NEO_GRB + NEO_KHZ800),
      numOfPixels(numOfPixels),
      currentState(NONE),
      blinkState(false)
{
  frontStrip.begin();
  frontStrip.clear();
  backStrip.begin();
  backStrip.clear();

  for (size_t i = 0; i < numOfPixels; i++)
  {
    originalColorsFront[i] = 0x000000; // Black color in RGB
    originalColorsBack[i] = 0x000000;  // Black color in RGB
  }
}

void Lighting::setStripColor(Adafruit_NeoPixel &strip, int r, int g, int b)
{
  for (size_t i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, r, g, b);
  }
  strip.setBrightness(255);
  strip.show();
}

void Lighting::parseRGB(String &input, int &r, int &g, int &b)
{
  r = input.substring(0, input.indexOf(":")).toInt();
  input.remove(0, input.indexOf(":") + 1);
  g = input.substring(0, input.indexOf(":")).toInt();
  input.remove(0, input.indexOf(":") + 1);
  b = input.toInt();
}

void Lighting::saveOriginalColors()
{
  // Save original colors for the front strip
  for (size_t i = 0; i < numOfPixels; i++)
  {
    originalColorsFront[i] = frontStrip.getPixelColor(i);
  }

  // Save original colors for the back strip
  for (size_t i = 0; i < numOfPixels; i++)
  {
    originalColorsBack[i] = backStrip.getPixelColor(i);
  }
}

void Lighting::restoreOriginalColors()
{
  // Restore original colors for the front strip
  for (size_t i = 0; i < numOfPixels; i++)
  {
    frontStrip.setPixelColor(i, originalColorsFront[i]);
  }

  // Restore original colors for the back strip
  for (size_t i = 0; i < numOfPixels; i++)
  {
    backStrip.setPixelColor(i, originalColorsBack[i]);
  }

  // Update the strips to reflect the changes
  frontStrip.show();
  backStrip.show();
}

void Lighting::startupAnimation()
{
  // Light blue color
  int r = 100, g = 70, b = 255;

  // Chase animation duration
  const int chaseDuration = 1500; // 1.5 seconds
  const int chaseDelay = chaseDuration / numOfPixels;

  // Breathing animation duration
  const int breatheDuration = 1500; // 1.5 seconds
  const int halfBreatheDuration = breatheDuration / 2;

  // Chase animation
  for (int pixel = 0; pixel < numOfPixels; pixel++)
  {
    frontStrip.setPixelColor(pixel, r, g, b);
    backStrip.setPixelColor(pixel, r, g, b);
    frontStrip.show();
    backStrip.show();
    delay(chaseDelay);
  }

  // Breathing animation
  for (int elapsed = 0; elapsed < breatheDuration; elapsed += 10)
  {
    delay(10); // Sleep for 10ms

    float alpha;
    if (elapsed <= halfBreatheDuration)
    {
      alpha = static_cast<float>(elapsed) / halfBreatheDuration; // Transition from 0 to 1 over 0.75 seconds
    }
    else
    {
      alpha = 1.0 - static_cast<float>(elapsed - halfBreatheDuration) / halfBreatheDuration; // Transition from 1 to 0 over 0.75 seconds
    }

    // Calculate the intensity based on alpha
    int currentR = r * alpha;
    int currentG = g * alpha;
    int currentB = b * alpha;

    setStripColor(frontStrip, currentR, currentG, currentB);
    setStripColor(backStrip, currentR, currentG, currentB);
  }

  setStripColor(frontStrip, 0, 0, 0);
  setStripColor(backStrip, 0, 0, 0);
}

void Lighting::control(String buffer)
{
  int separatorIndex = buffer.indexOf(":");
  String commandType = buffer.substring(0, separatorIndex);
  String command = buffer.substring(separatorIndex + 1);

  Serial.println(buffer);

  if (commandType == "lights")
  {
    if (command.startsWith("front:"))
    {
      command.remove(0, command.indexOf(":") + 1);
      int r, g, b;
      parseRGB(command, r, g, b);
      setStripColor(frontStrip, r, g, b);
      setState(State::NONE);
    }
    else if (command.startsWith("back:"))
    {
      command.remove(0, command.indexOf(":") + 1);
      int r, g, b;
      parseRGB(command, r, g, b);
      setStripColor(backStrip, r, g, b);
      setState(State::NONE);
    }
    else if (command.startsWith("all:"))
    {
      command.remove(0, command.indexOf(":") + 1);
      int r, g, b;
      parseRGB(command, r, g, b);
      setStripColor(frontStrip, r, g, b);
      setStripColor(backStrip, r, g, b);
      setState(State::NONE);
    }
    else if (command.startsWith("animation:"))
    {
      command.remove(0, command.indexOf(":") + 1);

      if (command == "off")
      {
        restoreOriginalColors();
        setState(NONE); // Turn off animations
      }
      else if (command == "blink_left")
      {
        saveOriginalColors();
        setState(BLINK_LEFT);
      }
      else if (command == "blink_right")
      {
        saveOriginalColors();
        setState(BLINK_RIGHT);
      }
      else if (command == "blink_all")
      {
        saveOriginalColors();
        setState(BLINK_ALL);
      }
      else if (command == "police")
      {
        saveOriginalColors();
        setState(POLICE_ANIMATION);
      }
    }
  }
}

void Lighting::setState(State newState)
{
  currentState = newState;
}

void Lighting::update()
{
  static unsigned long previousMillis = 0; // stores the last time the LED was updated
  long interval = 500;                     // interval at which to blink (milliseconds)
  const uint8_t BLINK_R = 255;
  const uint8_t BLINK_G = 50;
  const uint8_t BLINK_B = 0;

  const size_t BLINK_SIZE = numOfPixels / 4; // Number of pixels for each blinker

  unsigned long currentMillis = millis();

  switch (currentState)
  {
  case BLINK_LEFT:
    if (currentMillis - previousMillis >= interval)
    {
      previousMillis = currentMillis;

      if (blinkState)
      {
        for (size_t i = 0; i < BLINK_SIZE; i++)
        {
          frontStrip.setPixelColor(i, BLINK_R, BLINK_G, BLINK_B);
          backStrip.setPixelColor(i, BLINK_R, BLINK_G, BLINK_B);
        }
      }
      else
      {
        restoreOriginalColors();
      }

      blinkState = !blinkState;
      frontStrip.show();
      backStrip.show();
    }
    break;
  case BLINK_RIGHT:
    if (currentMillis - previousMillis >= interval)
    {
      previousMillis = currentMillis;
      if (blinkState)
      {
        for (size_t i = numOfPixels - BLINK_SIZE; i < numOfPixels; i++) // Starts from the last quarter of the strip
        {
          frontStrip.setPixelColor(i, BLINK_R, BLINK_G, BLINK_B);
          backStrip.setPixelColor(i, BLINK_R, BLINK_G, BLINK_B);
        }
      }
      else
      {
        restoreOriginalColors();
      }
      blinkState = !blinkState;
      frontStrip.show();
      backStrip.show();
    }
    break;
  case BLINK_ALL:
    if (currentMillis - previousMillis >= interval)
    {
      previousMillis = currentMillis;

      if (blinkState)
      {
        for (size_t i = 0; i < numOfPixels; i++)
        {
          // Set to orange color for all pixels
          frontStrip.setPixelColor(i, BLINK_R, BLINK_G, BLINK_B);
          backStrip.setPixelColor(i, BLINK_R, BLINK_G, BLINK_B);
        }
      }
      else
      {
        restoreOriginalColors();
      }

      blinkState = !blinkState;
      frontStrip.show();
      backStrip.show();
    }
    break;
  case POLICE_ANIMATION:
    interval = 60; // Increased the speed for a more aggressive look
    if (currentMillis - previousMillis >= interval)
    {
      previousMillis = currentMillis;

      const uint8_t flashes = 3;
      const uint8_t maxCycles = 10;
      static uint8_t cycleCount = 0;
      static uint8_t flashCount = 0;
      static bool isBlue = true;
      static int chaseIndex = 0;

      if (cycleCount < maxCycles)
      {
        // Flashing logic
        if (blinkState)
        {
          size_t startPixel = isBlue ? 0 : numOfPixels / 2;
          size_t endPixel = isBlue ? numOfPixels / 2 : numOfPixels;
          uint32_t color = isBlue ? frontStrip.Color(0, 0, 255) : frontStrip.Color(255, 0, 0);

          for (size_t i = startPixel; i < endPixel; i++)
          {
            frontStrip.setPixelColor(i, color);
            backStrip.setPixelColor(i, color);
          }
        }
        else
        {
          frontStrip.clear();
          backStrip.clear();
          flashCount++;

          if (flashCount >= flashes)
          {
            isBlue = !isBlue;
            flashCount = 0;
            cycleCount++;
          }
        }
      }
      else
      {
        // Enhanced chase logic
        frontStrip.clear();
        backStrip.clear();

        const int gap = 8;        // Distance between blue and red lights
        const int chaseWidth = 3; // Number of consecutive blue or red pixels
        for (int i = 0; i < chaseWidth; i++)
        {
          if (chaseIndex + i < numOfPixels)
          {
            frontStrip.setPixelColor(chaseIndex + i, 0, 0, 255);
            backStrip.setPixelColor(chaseIndex + i, 0, 0, 255);
          }

          if (chaseIndex - gap + i >= 0)
          {
            frontStrip.setPixelColor(chaseIndex - gap + i, 255, 0, 0);
            backStrip.setPixelColor(chaseIndex - gap + i, 255, 0, 0);
          }
        }

        chaseIndex += chaseWidth;
        if (chaseIndex >= numOfPixels + gap) // Making sure the chase goes beyond the strip for full clear
        {
          chaseIndex = 0;
          cycleCount = 0; // Reset to go back to flashing after one full chase
        }
      }

      blinkState = !blinkState;
      frontStrip.show();
      backStrip.show();
    }
    break;
  case NONE:
  default:
    // Do nothing or reset lights
    break;
  }
}
