#include "HardwareManager.h"

void HardwareManager::init() {
    Serial.println("[HAL] Hardware Abstraction Layer starting...");
    Serial.println("[HAL] Ready");
}

void HardwareManager::discoverSensors() {
    Serial.println("[HAL] Starting sensor discovery...");
    sensors.clear();
    discoverI2CSensors();
    discoverDigitalSensors();
    discoverAnalogSensors();
    Serial.printf("[HAL] Discovery complete: %d sensors found\n", sensors.size());
}

void HardwareManager::discoverI2CSensors() {
    Serial.println("[HAL] Scanning I2C bus...");
}

void HardwareManager::discoverDigitalSensors() {
    Serial.println("[HAL] Probing GPIO for digital sensors...");
}

void HardwareManager::discoverAnalogSensors() {
    Serial.println("[HAL] Scanning analog inputs...");
}

void HardwareManager::getSensorMapJSON(JsonArray& output) {
    for (const auto& sensor : sensors) {
        JsonObject sensorObj = output.add<JsonObject>();
        sensorObj["id"] = sensor->getID();
        sensorObj["type"] = sensor->getType();
        sensorObj["gpio"] = sensor->getGPIO();
        sensorObj["status"] = sensor->isConnected() ? "CONNECTED" : "DISCONNECTED";
        
        JsonObject caps = sensorObj["capabilities"].to<JsonObject>();
        sensor->getCapabilities(caps);
    }
}

void HardwareManager::getAvailableGPIO(JsonArray& output) {
    int allGPIO[] = {4, 5, 12, 13, 14, 15, 16, 17, 21, 22, 23};
    
    for (int pin : allGPIO) {
        if (isGPIOAvailable(pin)) {
            output.add(pin);
        }
    }
}

SensorBase* HardwareManager::getSensorByID(const String& id) {
    for (auto& sensor : sensors) {
        if (sensor->getID() == id) {
            return sensor.get();
        }
    }
    return nullptr;
}

int HardwareManager::getPowerBudgetRemaining() {
    int used = calculatePowerDraw();
    return 500 - used;
}

bool HardwareManager::isGPIOAvailable(int gpio) {
    if (isGPIOReserved(gpio)) return false;
    
    for (const auto& sensor : sensors) {
        if (sensor->getGPIO() == gpio) return false;
    }
    
    return true;
}

void HardwareManager::registerSensor(std::shared_ptr<SensorBase> sensor) {
    sensors.push_back(sensor);
    markGPIOUsed(sensor->getGPIO());
}

bool HardwareManager::isGPIOReserved(int gpio) {
    return std::find(reservedGPIO.begin(), reservedGPIO.end(), gpio) != reservedGPIO.end();
}

void HardwareManager::markGPIOUsed(int gpio) {
    if (!isGPIOReserved(gpio)) {
        reservedGPIO.push_back(gpio);
    }
}

int HardwareManager::calculatePowerDraw() {
    int total = 0;
    for (const auto& sensor : sensors) {
        total += sensor->getPowerDraw();
    }
    return total;
}
