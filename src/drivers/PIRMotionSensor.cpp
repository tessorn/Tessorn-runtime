#include "PIRMotionSensor.h"
#include "../communication/Protocol.h"

PIRMotionSensor* PIRMotionSensor::instance = nullptr;

PIRMotionSensor::PIRMotionSensor(int gpio) : gpio(gpio) {
    instance = this;
}

String PIRMotionSensor::getID() const {
    return "pir_motion_" + String(gpio);
}

String PIRMotionSensor::getType() const {
    return "PIR_MOTION";
}

void PIRMotionSensor::getCapabilities(JsonObject& caps) const {
    caps["detection_range_meters"] = 7;
    caps["detection_angle_degrees"] = 110;
    caps["supports_sensitivity"] = true;
    caps["cooldown_min_ms"] = 200;
    caps["cooldown_max_ms"] = 10000;
}

bool PIRMotionSensor::probe() {
    pinMode(gpio, INPUT);
    delay(100);
    
    int readings = 0;
    for (int i = 0; i < 10; i++) {
        readings += digitalRead(gpio);
        delay(50);
    }
    
    if (readings > 0 && readings < 10) {
        connected = true;
        return true;
    }
    
    return false;
}

void PIRMotionSensor::init() {
    pinMode(gpio, INPUT);
    connected = true;
    Serial.printf("[PIR] Initialized on GPIO %d\n", gpio);
}

void PIRMotionSensor::update() {
    bool currentState = digitalRead(gpio);
    unsigned long now = millis();
    
    if (currentState != lastState) {
        lastState = currentState;
        
        if (currentState == HIGH) {
            if (now - lastTriggerTime > cooldownMs) {
                lastTriggerTime = now;
                
                JsonDocument doc;
                JsonObject data = doc.to<JsonObject>();
                data["sensor_id"] = getID();
                data["gpio"] = gpio;
                data["motion_detected"] = true;
                
                triggerEvent("MOTION_DETECTED", data);
                Protocol::sendDebug("Motion detected on GPIO " + String(gpio));
            }
        } else {
            JsonDocument doc;
            JsonObject data = doc.to<JsonObject>();
            data["sensor_id"] = getID();
            data["gpio"] = gpio;
            data["motion_detected"] = false;
            
            triggerEvent("MOTION_ENDED", data);
        }
    }
}

void PIRMotionSensor::cleanup() {
    connected = false;
}

void PIRMotionSensor::setSensitivity(int sens) {
    sensitivity = constrain(sens, 0, 100);
}

void PIRMotionSensor::setCooldown(unsigned long cooldown) {
    cooldownMs = constrain(cooldown, 200, 10000);
}

void IRAM_ATTR PIRMotionSensor::handleInterrupt() {
}
