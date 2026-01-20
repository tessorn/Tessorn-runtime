#ifndef BEHAVIOR_ENGINE_H
#define BEHAVIOR_ENGINE_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <vector>
#include <map>
#include "../communication/Protocol.h"

struct Trigger {
    String sensorID;
    String eventType;
    JsonObject conditions;
};

struct Action {
    String deviceID;
    String actionType;
    JsonObject parameters;
};

struct Behavior {
    String id;
    String name;
    bool active;
    std::vector<Trigger> triggers;
    std::vector<Action> actions;
    unsigned long cooldownMs;
    unsigned long lastExecutionTime;
};

enum EngineState {
    ENGINE_IDLE,
    ENGINE_RUNNING,
    ENGINE_ERROR
};

class BehaviorEngine {
public:
    void init();
    void tick();
    
    String loadBehavior(const JsonDocument& profile);
    bool startBehavior(const String& behaviorID);
    void stopBehavior(const String& behaviorID);
    void stopAllBehaviors();
    
    ValidationResult validateBehavior(const JsonDocument& profile);
    
    int getActiveBehaviorCount() const;
    String getStateString() const;
    EngineState getState() const { return state; }
    
    void handleSensorEvent(const String& sensorID, const String& eventType, JsonObject& data);
    
private:
    EngineState state = ENGINE_IDLE;
    std::map<String, Behavior> behaviors;
    
    bool parseBehavior(const JsonDocument& profile, Behavior& behavior);
    bool checkTriggerConditions(const Trigger& trigger, JsonObject& eventData);
    void executeActions(const std::vector<Action>& actions);
    void executeAction(const Action& action);
    
    ValidationResult validateTriggers(const JsonArray& triggers);
    ValidationResult validateActions(const JsonArray& actions);
    ValidationResult checkPowerBudget(const JsonArray& actions);
    ValidationResult checkGPIOAvailability(const JsonArray& triggers, const JsonArray& actions);
};

#endif
