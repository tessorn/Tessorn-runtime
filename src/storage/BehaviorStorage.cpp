#include "BehaviorStorage.h"
#include "../core/TessornCore.h"
#include "../../include/config.h"

void BehaviorStorage::init() {
    if (!LittleFS.begin(true)) {
        Serial.println("[STORAGE] Failed to mount filesystem");
        return;
    }
    
    Serial.println("[STORAGE] Filesystem mounted");
    
    if (!ensureStorageDirectory()) {
        Serial.println("[STORAGE] Failed to create storage directory");
    }
    
    size_t totalBytes = LittleFS.totalBytes();
    size_t usedBytes = LittleFS.usedBytes();
    Serial.printf("[STORAGE] Total: %d KB | Used: %d KB | Free: %d KB\n",
                  totalBytes / 1024, usedBytes / 1024, (totalBytes - usedBytes) / 1024);
}

bool BehaviorStorage::saveBehavior(const String& behaviorID, const JsonDocument& profile) {
    String path = getBehaviorPath(behaviorID);
    
    File file = LittleFS.open(path, "w");
    if (!file) {
        Serial.printf("[STORAGE] Failed to open file for writing: %s\n", path.c_str());
        return false;
    }
    
    size_t written = serializeJson(profile, file);
    file.close();
    
    if (written == 0) {
        Serial.println("[STORAGE] Failed to write behavior");
        return false;
    }
    
    Serial.printf("[STORAGE] Behavior '%s' saved (%d bytes)\n", behaviorID.c_str(), written);
    return true;
}

bool BehaviorStorage::loadBehavior(const String& behaviorID, JsonDocument& profile) {
    String path = getBehaviorPath(behaviorID);
    
    if (!LittleFS.exists(path)) {
        Serial.printf("[STORAGE] Behavior file not found: %s\n", path.c_str());
        return false;
    }
    
    File file = LittleFS.open(path, "r");
    if (!file) {
        Serial.printf("[STORAGE] Failed to open file for reading: %s\n", path.c_str());
        return false;
    }
    
    DeserializationError error = deserializeJson(profile, file);
    file.close();
    
    if (error) {
        Serial.printf("[STORAGE] Failed to parse behavior: %s\n", error.c_str());
        return false;
    }
    
    Serial.printf("[STORAGE] Behavior '%s' loaded\n", behaviorID.c_str());
    return true;
}

bool BehaviorStorage::deleteBehavior(const String& behaviorID) {
    String path = getBehaviorPath(behaviorID);
    
    if (!LittleFS.exists(path)) {
        return false;
    }
    
    bool success = LittleFS.remove(path);
    
    if (success) {
        Serial.printf("[STORAGE] Behavior '%s' deleted\n", behaviorID.c_str());
    }
    
    return success;
}

int BehaviorStorage::loadAllBehaviors() {
    return 0;
}

void BehaviorStorage::listBehaviors(JsonArray& output) {
}

void BehaviorStorage::clearAll() {
    Serial.println("[STORAGE] All behaviors cleared");
}

String BehaviorStorage::getBehaviorPath(const String& behaviorID) {
    return String(BEHAVIOR_STORAGE_PATH) + "/" + behaviorID + ".json";
}

bool BehaviorStorage::ensureStorageDirectory() {
    if (!LittleFS.exists(BEHAVIOR_STORAGE_PATH)) {
        return LittleFS.mkdir(BEHAVIOR_STORAGE_PATH);
    }
    return true;
}
