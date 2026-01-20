#include "TessornCore.h"
#include "../../include/config.h"

TessornCore& TessornCore::getInstance() {
    static TessornCore instance;
    return instance;
}

void TessornCore::init() {
    Serial.begin(SERIAL_BAUD_RATE);
    while (!Serial && millis() < 5000) {
        delay(10);
    }
    
    Serial.println("\n\n═══════════════════════════════════════");
    Serial.println("    TESSORN RUNTIME ENGINE v" TESSORN_RUNTIME_VERSION);
    Serial.println("═══════════════════════════════════════");
    
    Serial.println("[CORE] Initializing storage...");
    storage.init();
    
    Serial.println("[CORE] Initializing hardware manager...");
    hardware.init();
    
    Serial.println("[CORE] Initializing behavior engine...");
    behaviorEngine.init();
    
    Serial.println("[CORE] Initializing protocol handler...");
    protocol.init();
    
    Serial.println("[CORE] System ready!");
    Serial.printf("[CORE] Chip: %s | Free Heap: %d KB\n", 
                  getChipModel().c_str(), 
                  getFreeHeap() / 1024);
    Serial.println("═══════════════════════════════════════\n");
    
    int loaded = storage.loadAllBehaviors();
    if (loaded > 0) {
        Serial.printf("[CORE] Loaded %d stored behaviors\n", loaded);
    }
}

void TessornCore::loop() {
    protocol.processIncoming();
    behaviorEngine.tick();
    checkHeartbeat();
    delay(1);
}

void TessornCore::checkHeartbeat() {
    unsigned long now = millis();
    if (now - lastHeartbeat > HEARTBEAT_INTERVAL_MS) {
        Serial.printf("[HEARTBEAT] Uptime: %lu s | Heap: %d KB | Active Behaviors: %d\n",
                      getUptime(), getFreeHeap() / 1024, 
                      behaviorEngine.getActiveBehaviorCount());
        lastHeartbeat = now;
    }
}

String TessornCore::getChipID() {
    uint64_t mac = ESP.getEfuseMac();
    char chipID[13];
    sprintf(chipID, "%04X%08X", (uint16_t)(mac >> 32), (uint32_t)mac);
    return String(chipID);
}

String TessornCore::getChipModel() {
    return String(ESP.getChipModel());
}

uint32_t TessornCore::getFreeHeap() {
    return ESP.getFreeHeap();
}

uint32_t TessornCore::getUptime() {
    return millis() / 1000;
}
