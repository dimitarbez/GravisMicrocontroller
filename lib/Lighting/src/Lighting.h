#include <Adafruit_NeoPixel.h>

class Lighting
{
public:
    enum State {
        NONE,
        BLINK_LEFT,
        BLINK_RIGHT,
        BLINK_ALL,
        POLICE_ANIMATION
    };

private:
    Adafruit_NeoPixel frontStrip;
    Adafruit_NeoPixel backStrip;
    uint32_t originalColorsFront[20];
    uint32_t originalColorsBack[20];
    int numOfPixels;
    State currentState;  // added to keep track of current animation state
    bool blinkState;


    void setStripColor(Adafruit_NeoPixel &strip, int r, int g, int b); // Added a generic function
    void parseRGB(String &input, int &r, int &g, int &b);
    void restoreOriginalColors();
    void saveOriginalColors();
    void setState(State newState); // Setter for current state

public:
    Lighting(int frontStripPin, int backStripPin, int numOfPixels = 20);
    void control(String buffer);
    void update();
};
