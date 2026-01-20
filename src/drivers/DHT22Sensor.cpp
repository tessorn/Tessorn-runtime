#include "DHT22Sensor.h"
#include "../communication/Protocol.h"

DHT22Sensor::DHT22Sensor(int gpio) : gpio(gpio) {
    dht = new DHT(gpio, DHT22);
}

String DHT22Sensor::getID() const {
    return "dht22_" + String(gpio);
}

String DHT22Sensor::getType() const {
    return "TEMPERATURE_HUMIDITY";
}

void DHT22Sensor::getCapabilities(JsonObject& caps) const {
    caps["polling_rate_ms"] = 2000;
    caps["accuracy_temp"] = 0.5;
    caps["accuracy_humidity"] = 2.0;
}

bool DHT22Sensor::probe() {
    dht->begin();
    delay(2000);
    
    float temp = dht->readTemperature();
    float humidity = dht->readHumidity();
    
    if (!isnan(temp) && !isnan(humidity)) {
        connected = true;
        lastTemp = temp;
        lastHumidity = humidity;
        return true;
    }
    
    return false;
}

void DHT22Sensor::init() {
    dht->begin();
    connected = true;
    Serial.printf("[DHT22] Initialized on GPIO %d\n", gpio);
}

void DHT22Sensor::update() {
    unsigned long now = millis();
    
    if (now - lastReadTime < readInterval) return;
    
    lastReadTime = now;
    
    float temp = dht->readTemperature();
    float humidity = dht->readHumidity();
    
    if (isnan(temp) || isnan(humidity)) {
        connected = false;
        Protocol::sendDebug("DHT22 read error on GPIO " + String(gpio));
        return;
    }
    
    if (abs(temp - lastTemp) > 0.5 || abs(humidity - lastHumidity) > 2.0) {
        lastTemp = temp;
        lastHumidity = humidity;
        
        JsonDocument doc;
        JsonObject data = doc.to<JsonObject>();
        data["sensor_id"] = getID();
        data["temperature"] = temp;
        data["humidity"] = humidity;
        
        triggerEvent("READING_UPDATE", data);
    }
}

void DHT22Sensor::cleanup() {
    delete dht;
    dht = nullptr;
    connected = false;
}
