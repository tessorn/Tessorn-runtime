#ifndef SENSOR_BASE_H
#define SENSOR_BASE_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <functional>

enum SensorType {
    SENSOR_DIGITAL,
    SENSOR_ANALOG,
    SENSOR_I2C,
    SENSOR_ACTUATOR
};

class SensorBase {
public:
    virtual ~SensorBase() = default;
    
    virtual String getID() const = 0;
    virtual String getType() const = 0;
    virtual int getGPIO() const = 0;
    
    virtual void getCapabilities(JsonObject& caps) const = 0;
    virtual int getPowerDraw() const = 0;
    
    virtual bool isConnected() const = 0;
    virtual bool probe() = 0;
    
    using EventCallback = std::function<void(const String& eventType, JsonObject& data)>;
    
    virtual void setEventCallback(EventCallback callback) {
        eventCallback = callback;
    }
    
    virtual void init() = 0;
    virtual void update() = 0;
    virtual void cleanup() = 0;
    
protected:
    EventCallback eventCallback;
    
    void triggerEvent(const String& eventType, JsonObject& data) {
        if (eventCallback) {
            eventCallback(eventType, data);
        }
    }
};

#endif
