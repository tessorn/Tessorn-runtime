#ifndef BUTTON_SENSOR_H
#define BUTTON_SENSOR_H

#include "SensorBase.h"

class ButtonSensor : public SensorBase {
public:
    explicit ButtonSensor(int gpio);
    
    String getID() const override;
    String getType() const override;
    int getGPIO() const override { return gpio; }
    
    void getCapabilities(JsonObject& caps) const override;
    int getPowerDraw() const override { return 0; }
    
    bool isConnected() const override { return connected; }
    bool probe() override;
    
    void init() override;
    void update() override;
    void cleanup() override;
    
private:
    int gpio;
    bool connected = false;
    bool lastState = false;
    unsigned long lastDebounceTime = 0;
    unsigned long debounceDelay = 50;
};

#endif
