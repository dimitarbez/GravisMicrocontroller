#include "Lighting.h"

Lighting::Lighting(int frontStripPin, int backStripPin, int numOfPixels)
    : frontStrip(numOfPixels, frontStripPin, NEO_GRB + NEO_KHZ800),
      backStrip(numOfPixels, backStripPin, NEO_GRB + NEO_KHZ800),
      numOfPixels(numOfPixels),
      currentState(NONE)
{
  frontStrip.begin();
  frontStrip.clear();
  backStrip.begin();
  backStrip.clear();
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

void Lighting::control(String buffer)
{
  int separatorIndex = buffer.indexOf(":");
  String commandType = buffer.substring(0, separatorIndex);
  String command = buffer.substring(separatorIndex + 1);

  if (commandType == "lights")
  {
    if (command.startsWith("front:"))
    {
      command.remove(0, command.indexOf(":") + 1);
      int r, g, b;
      parseRGB(command, r, g, b);
      setStripColor(frontStrip, r, g, b);
    }
    else if (command.startsWith("back:"))
    {
      command.remove(0, command.indexOf(":") + 1);
      int r, g, b;
      parseRGB(command, r, g, b);
      setStripColor(backStrip, r, g, b);
    }
    else if (command.startsWith("all:"))
    {
      command.remove(0, command.indexOf(":") + 1);
      int r, g, b;
      parseRGB(command, r, g, b);
      setStripColor(frontStrip, r, g, b);
      setStripColor(backStrip, r, g, b);
    }
    else if (command.startsWith("animation:"))
    {
      command.remove(0, command.indexOf(":") + 1);

      if (command == "off")
      {
        setState(NONE); // Turn off animations
      }
      else if (command == "blink_left")
      {
        setState(BLINK_LEFT);
      }
      else if (command == "blink_right")
      {
        setState(BLINK_RIGHT);
      }
      else if (command == "blink_all")
      {
        setState(BLINK_ALL);
      }
      else if (command == "police")
      {
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
  switch (currentState)
  {
  case BLINK_LEFT:
    // Your logic to handle blink left animation
    break;
  case BLINK_RIGHT:
    // Your logic to handle blink right animation
    break;
  case BLINK_ALL:
    // Your logic to handle blink all animation
    break;
  case POLICE_ANIMATION:
    // Your logic to handle police animation
    break;
  case NONE:
  default:
    // Do nothing or reset lights
    break;
  }
}
