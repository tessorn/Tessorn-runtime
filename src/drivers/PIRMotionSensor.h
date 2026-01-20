#ifndef PIR_MOTION_SENSOR_H
#define PIR_MOTION_SENSOR_H

#include "SensorBase.h"

class PIRMotionSensor : public SensorBase {
public:
    explicit PIRMotionSensor(int gpio);
    
    String getID() const override;
    String getType() const override;
    int getGPIO() const override { return gpio; }
    
    void getCapabilities(JsonObject& caps) const override;
    int getPowerDraw() const override { return 65; }
    
    bool isConnected() const override { return connected; }
    bool probe() override;
    
    void init() override;
    void update() override;
    void cleanup() override;
    
    void setSensitivity(int sensitivity);
    void setCooldown(unsigned long cooldownMs);
    
private:
    int gpio;
    bool connected = false;
    int sensitivity = 50;
    unsigned long cooldownMs = 2000;
    unsigned long lastTriggerTime = 0;
    bool lastState = false;
    
    static void IRAM_ATTR handleInterrupt();
    static PIRMotionSensor* instance;
};

#endif
