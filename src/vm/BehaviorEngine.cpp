#include "BehaviorEngine.h"
#include "../core/TessornCore.h"

void BehaviorEngine::init() {
    state = ENGINE_RUNNING;
    Serial.println("[ENGINE] Behavior Engine initialized");
}

void BehaviorEngine::tick() {
    if (state != ENGINE_RUNNING) return;
}

String BehaviorEngine::loadBehavior(const JsonDocument& profile) {
    Behavior behavior;
    
    if (!parseBehavior(profile, behavior)) {
        Serial.println("[ENGINE] Failed to parse behavior");
        return "";
    }
    
    behaviors[behavior.id] = behavior;
    
    Serial.printf("[ENGINE] Behavior '%s' loaded\n", behavior.id.c_str());
    return behavior.id;
}

bool BehaviorEngine::startBehavior(const String& behaviorID) {
    auto it = behaviors.find(behaviorID);
    if (it == behaviors.end()) {
        Serial.printf("[ENGINE] Behavior '%s' not found\n", behaviorID.c_str());
        return false;
    }
    
    it->second.active = true;
    Serial.printf("[ENGINE] Behavior '%s' started\n", behaviorID.c_str());
    
    return true;
}

void BehaviorEngine::stopBehavior(const String& behaviorID) {
    auto it = behaviors.find(behaviorID);
    if (it != behaviors.end()) {
        it->second.active = false;
        Serial.printf("[ENGINE] Behavior '%s' stopped\n", behaviorID.c_str());
    }
}

void BehaviorEngine::stopAllBehaviors() {
    for (auto& pair : behaviors) {
        pair.second.active = false;
    }
    Serial.println("[ENGINE] All behaviors stopped");
}

ValidationResult BehaviorEngine::validateBehavior(const JsonDocument& profile) {
    ValidationResult result;
    result.passed = true;
    
    if (!profile.containsKey("id") || !profile.containsKey("triggers") || !profile.containsKey("actions")) {
        result.passed = false;
        result.errorMessage = "Missing required fields";
        return result;
    }
    
    return result;
}

bool BehaviorEngine::parseBehavior(const JsonDocument& profile, Behavior& behavior) {
    behavior.id = profile["id"].as<String>();
    behavior.name = profile["name"].as<String>();
    behavior.active = false;
    behavior.cooldownMs = profile["cooldown_ms"] | 0;
    behavior.lastExecutionTime = 0;
    
    return true;
}

int BehaviorEngine::getActiveBehaviorCount() const {
    int count = 0;
    for (const auto& pair : behaviors) {
        if (pair.second.active) count++;
    }
    return count;
}

String BehaviorEngine::getStateString() const {
    switch (state) {
        case ENGINE_IDLE: return "IDLE";
        case ENGINE_RUNNING: return "RUNNING";
        case ENGINE_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

ValidationResult BehaviorEngine::validateTriggers(const JsonArray& triggers) {
    ValidationResult result;
    result.passed = true;
    return result;
}

ValidationResult BehaviorEngine::validateActions(const JsonArray& actions) {
    ValidationResult result;
    result.passed = true;
    return result;
}

ValidationResult BehaviorEngine::checkPowerBudget(const JsonArray& actions) {
    ValidationResult result;
    result.passed = true;
    return result;
}

ValidationResult BehaviorEngine::checkGPIOAvailability(const JsonArray& triggers, const JsonArray& actions) {
    ValidationResult result;
    result.passed = true;
    return result;
}

bool BehaviorEngine::checkTriggerConditions(const Trigger& trigger, JsonObject& eventData) {
    return true;
}

void BehaviorEngine::executeActions(const std::vector<Action>& actions) {
    for (const auto& action : actions) {
        executeAction(action);
    }
}

void BehaviorEngine::executeAction(const Action& action) {
    Serial.printf("[ENGINE] Executing action: %s\n", action.actionType.c_str());
}

void BehaviorEngine::handleSensorEvent(const String& sensorID, const String& eventType, JsonObject& data) {
    Serial.printf("[ENGINE] Event: %s from %s\n", eventType.c_str(), sensorID.c_str());
}
