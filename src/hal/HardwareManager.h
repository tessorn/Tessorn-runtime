#ifndef HARDWARE_MANAGER_H
#define HARDWARE_MANAGER_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <vector>
#include <memory>
#include "../drivers/SensorBase.h"

class HardwareManager {
public:
    void init();
    void discoverSensors();
    void getSensorMapJSON(JsonArray& output);
    void getAvailableGPIO(JsonArray& output);
    
    SensorBase* getSensorByID(const String& id);
    int getPowerBudgetRemaining();
    bool isGPIOAvailable(int gpio);
    
    void registerSensor(std::shared_ptr<SensorBase> sensor);
    
private:
    std::vector<std::shared_ptr<SensorBase>> sensors;
    std::vector<int> reservedGPIO = {18, 19};
    
    void discoverI2CSensors();
    void discoverDigitalSensors();
    void discoverAnalogSensors();
    
    bool isGPIOReserved(int gpio);
    void markGPIOUsed(int gpio);
    int calculatePowerDraw();
};

#endif
