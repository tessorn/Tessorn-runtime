#include "ButtonSensor.h"
#include "../communication/Protocol.h"

ButtonSensor::ButtonSensor(int gpio) : gpio(gpio) {}

String ButtonSensor::getID() const {
    return "button_" + String(gpio);
}

String ButtonSensor::getType() const {
    return "BUTTON";
}

void ButtonSensor::getCapabilities(JsonObject& caps) const {
    caps["supports_press"] = true;
    caps["supports_release"] = true;
    caps["debounce_ms"] = debounceDelay;
}

bool ButtonSensor::probe() {
    pinMode(gpio, INPUT_PULLUP);
    delay(50);
    bool state = digitalRead(gpio);
    if (state == LOW) {
        connected = true;
        return true;
    }
    return false;
}

void ButtonSensor::init() {
    pinMode(gpio, INPUT_PULLUP);
    lastState = digitalRead(gpio);
    connected = true;
    Serial.printf("[BUTTON] Initialized on GPIO %d\n", gpio);
}

void ButtonSensor::update() {
    bool currentState = digitalRead(gpio);
    unsigned long now = millis();
    
    if (currentState != lastState) {
        lastDebounceTime = now;
    }
    
    if ((now - lastDebounceTime) > debounceDelay) {
        if (currentState != lastState) {
            lastState = currentState;
            
            JsonDocument doc;
            JsonObject data = doc.to<JsonObject>();
            data["sensor_id"] = getID();
            data["gpio"] = gpio;
            
            if (currentState == LOW) {
                data["pressed"] = true;
                triggerEvent("BUTTON_PRESSED", data);
                Protocol::sendDebug("Button pressed on GPIO " + String(gpio));
            } else {
                data["pressed"] = false;
                triggerEvent("BUTTON_RELEASED", data);
                Protocol::sendDebug("Button released on GPIO " + String(gpio));
            }
        }
    }
}

void ButtonSensor::cleanup() {
    connected = false;
}
