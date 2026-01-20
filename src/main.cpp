#include <Arduino.h>
#include "core/TessornCore.h"
#include "../include/config.h"

TessornCore* core = nullptr;

void setup() {
    core = &TessornCore::getInstance();
    core->init();
    
    Serial.println("\n╔════════════════════════════════════════╗");
    Serial.println("║   TESSORN RUNTIME ENGINE READY         ║");
    Serial.println("║   Awaiting FlowLabs IDE Connection...  ║");
    Serial.println("╚════════════════════════════════════════╝\n");
}

void loop() {
    if (core) {
        core->loop();
    }
}
