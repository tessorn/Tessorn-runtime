#ifndef BEHAVIOR_STORAGE_H
#define BEHAVIOR_STORAGE_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>

class BehaviorStorage {
public:
    void init();
    
    bool saveBehavior(const String& behaviorID, const JsonDocument& profile);
    bool loadBehavior(const String& behaviorID, JsonDocument& profile);
    bool deleteBehavior(const String& behaviorID);
    
    int loadAllBehaviors();
    void listBehaviors(JsonArray& output);
    
    void clearAll();
    
private:
    String getBehaviorPath(const String& behaviorID);
    bool ensureStorageDirectory();
};

#endif
