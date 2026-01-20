#ifndef DHT22_SENSOR_H
#define DHT22_SENSOR_H

#include "SensorBase.h"
#include <DHT.h>

class DHT22Sensor : public SensorBase {
public:
    explicit DHT22Sensor(int gpio);
    
    String getID() const override;
    String getType() const override;
    int getGPIO() const override { return gpio; }
    
    void getCapabilities(JsonObject& caps) const override;
    int getPowerDraw() const override { return 2; }
    
    bool isConnected() const override { return connected; }
    bool probe() override;
    
    void init() override;
    void update() override;
    void cleanup() override;
    
    float getTemperature() const { return lastTemp; }
    float getHumidity() const { return lastHumidity; }
    
private:
    int gpio;
    DHT* dht = nullptr;
    bool connected = false;
    unsigned long lastReadTime = 0;
    unsigned long readInterval = 2000;
    
    float lastTemp = 0;
    float lastHumidity = 0;
};

#endif
