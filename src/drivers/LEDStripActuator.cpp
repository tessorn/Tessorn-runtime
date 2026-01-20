#include "LEDStripActuator.h"
#include "../communication/Protocol.h"

LEDStripActuator::LEDStripActuator(int gpio, int numLeds) 
    : gpio(gpio), numLeds(constrain(numLeds, 1, MAX_LEDS)) {
    leds = new CRGB[numLeds];
}

String LEDStripActuator::getID() const {
    return "led_strip_" + String(gpio);
}

String LEDStripActuator::getType() const {
    return "LED_STRIP_WS2812";
}

void LEDStripActuator::getCapabilities(JsonObject& caps) const {
    caps["pixel_count"] = numLeds;
    caps["max_brightness"] = 255;
    caps["supports_rgb"] = true;
    caps["supports_fade"] = true;
    caps["fade_resolution_ms"] = 10;
    caps["max_update_rate_hz"] = 400;
}

int LEDStripActuator::getPowerDraw() const {
    return numLeds * 30;
}

bool LEDStripActuator::probe() {
    return true;
}

void LEDStripActuator::init() {
    FastLED.addLeds<WS2812B, 5, GRB>(leds, numLeds);
    FastLED.setBrightness(128);
    fill_solid(leds, numLeds, CRGB::Black);
    FastLED.show();
    
    connected = true;
    Serial.printf("[LED] Strip initialized on GPIO %d (%d LEDs)\n", gpio, numLeds);
}

void LEDStripActuator::update() {
    if (fading) {
        updateFade();
    }
}

void LEDStripActuator::cleanup() {
    turnOff();
    delete[] leds;
    leds = nullptr;
    connected = false;
}

void LEDStripActuator::setColor(uint8_t r, uint8_t g, uint8_t b) {
    fill_solid(leds, numLeds, CRGB(r, g, b));
    FastLED.show();
    Protocol::sendDebug("LED strip set to RGB(" + String(r) + "," + String(g) + "," + String(b) + ")");
}

void LEDStripActuator::setBrightness(uint8_t brightness) {
    FastLED.setBrightness(brightness);
    FastLED.show();
}

void LEDStripActuator::fadeToColor(uint8_t r, uint8_t g, uint8_t b, unsigned long durationMs) {
    fadeStartColor = leds[0];
    fadeEndColor = CRGB(r, g, b);
    fadeStartTime = millis();
    fadeDuration = durationMs;
    fading = true;
    Protocol::sendDebug("LED fade started: " + String(durationMs) + "ms");
}

void LEDStripActuator::fadeIn(unsigned long durationMs) {
    fadeToColor(255, 255, 255, durationMs);
}

void LEDStripActuator::fadeOut(unsigned long durationMs) {
    fadeToColor(0, 0, 0, durationMs);
}

void LEDStripActuator::turnOff() {
    fill_solid(leds, numLeds, CRGB::Black);
    FastLED.show();
    fading = false;
}

void LEDStripActuator::updateFade() {
    unsigned long now = millis();
    unsigned long elapsed = now - fadeStartTime;
    
    if (elapsed >= fadeDuration) {
        fill_solid(leds, numLeds, fadeEndColor);
        FastLED.show();
        fading = false;
        Protocol::sendDebug("LED fade complete");
        return;
    }
    
    float progress = (float)elapsed / fadeDuration;
    
    uint8_t r = fadeStartColor.r + (fadeEndColor.r - fadeStartColor.r) * progress;
    uint8_t g = fadeStartColor.g + (fadeEndColor.g - fadeStartColor.g) * progress;
    uint8_t b = fadeStartColor.b + (fadeEndColor.b - fadeStartColor.b) * progress;
    
    fill_solid(leds, numLeds, CRGB(r, g, b));
    FastLED.show();
}
