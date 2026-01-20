#ifndef TESSORN_CORE_H
#define TESSORN_CORE_H

#include <Arduino.h>
#include "../communication/Protocol.h"
#include "../hal/HardwareManager.h"
#include "../vm/BehaviorEngine.h"
#include "../storage/BehaviorStorage.h"

class TessornCore {
public:
    static TessornCore& getInstance();
    
    void init();
    void loop();
    
    Protocol& getProtocol() { return protocol; }
    HardwareManager& getHardware() { return hardware; }
    BehaviorEngine& getBehaviorEngine() { return behaviorEngine; }
    BehaviorStorage& getStorage() { return storage; }
    
    String getChipID();
    String getChipModel();
    uint32_t getFreeHeap();
    uint32_t getUptime();
    
private:
    TessornCore() = default;
    TessornCore(const TessornCore&) = delete;
    TessornCore& operator=(const TessornCore&) = delete;
    
    Protocol protocol;
    HardwareManager hardware;
    BehaviorEngine behaviorEngine;
    BehaviorStorage storage;
    
    unsigned long lastHeartbeat = 0;
    void checkHeartbeat();
};

#endif
