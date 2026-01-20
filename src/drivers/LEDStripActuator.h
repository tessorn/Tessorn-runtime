#ifndef LED_STRIP_ACTUATOR_H
#define LED_STRIP_ACTUATOR_H

#include "SensorBase.h"
#include <FastLED.h>

#define MAX_LEDS 300

class LEDStripActuator : public SensorBase {
public:
    LEDStripActuator(int gpio, int numLeds = 60);
    
    String getID() const override;
    String getType() const override;
    int getGPIO() const override { return gpio; }
    
    void getCapabilities(JsonObject& caps) const override;
    int getPowerDraw() const override;
    
    bool isConnected() const override { return connected; }
    bool probe() override;
    
    void init() override;
    void update() override;
    void cleanup() override;
    
    void setColor(uint8_t r, uint8_t g, uint8_t b);
    void setBrightness(uint8_t brightness);
    void fadeToColor(uint8_t r, uint8_t g, uint8_t b, unsigned long durationMs);
    void fadeIn(unsigned long durationMs);
    void fadeOut(unsigned long durationMs);
    void turnOff();
    
private:
    int gpio;
    int numLeds;
    CRGB* leds = nullptr;
    bool connected = false;
    
    bool fading = false;
    unsigned long fadeStartTime = 0;
    unsigned long fadeDuration = 0;
    CRGB fadeStartColor;
    CRGB fadeEndColor;
    
    void updateFade();
};

#endif
