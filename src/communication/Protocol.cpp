#include "Protocol.h"
#include "../core/TessornCore.h"
#include "../../include/config.h"

void Protocol::init() {
    Serial.println("[PROTOCOL] Communication layer initialized");
}

void Protocol::processIncoming() {
    if (!Serial.available()) return;
    
    String cmdStr = readCommand();
    if (cmdStr.length() == 0) return;
    
    JsonDocument cmd;
    DeserializationError error = deserializeJson(cmd, cmdStr);
    
    if (error) {
        sendError("Invalid JSON: " + String(error.c_str()));
        return;
    }
    
    if (!cmd.containsKey("cmd")) {
        sendError("Missing 'cmd' field");
        return;
    }
    
    handleCommand(cmd);
}

String Protocol::readCommand() {
    String buffer = "";
    unsigned long startTime = millis();
    
    while (millis() - startTime < COMMAND_TIMEOUT_MS) {
        if (Serial.available()) {
            char c = Serial.read();
            if (c == '\n') {
                return buffer;
            }
            buffer += c;
        }
        delay(1);
    }
    
    return buffer;
}

void Protocol::handleCommand(const JsonDocument& cmd) {
    CommandType cmdType = (CommandType)cmd["cmd"].as<int>();
    JsonDocument payload = cmd["payload"];
    
    switch (cmdType) {
        case CMD_HANDSHAKE:
            handleHandshake(payload);
            break;
        case CMD_GET_SENSOR_MAP:
            handleGetSensorMap(payload);
            break;
        case CMD_PING:
            handlePing(payload);
            break;
        default:
            sendError("Unknown command", STATUS_INVALID_COMMAND);
            break;
    }
}

void Protocol::handleHandshake(const JsonDocument& payload) {
    auto& core = TessornCore::getInstance();
    
    JsonDocument response;
    response["status"] = "OK";
    response["runtime_version"] = TESSORN_RUNTIME_VERSION;
    response["protocol_version"] = PROTOCOL_VERSION;
    response["chip_id"] = core.getChipID();
    response["chip_model"] = core.getChipModel();
    response["free_heap_kb"] = core.getFreeHeap() / 1024;
    response["vm_ready"] = true;
    
    sendSuccess(response);
    Serial.println("[PROTOCOL] Handshake completed");
}

void Protocol::handleGetSensorMap(const JsonDocument& payload) {
    JsonDocument response;
    response["sensors"] = JsonArray();
    response["message"] = "Sensor discovery not fully implemented";
    sendSuccess(response);
}

void Protocol::handlePing(const JsonDocument& payload) {
    JsonDocument response;
    response["pong"] = true;
    response["timestamp"] = millis();
    sendSuccess(response);
}

void Protocol::sendSuccess(const JsonDocument& data) {
    JsonDocument response;
    response["status"] = STATUS_OK;
    response["data"] = data;
    
    serializeJson(response, Serial);
    Serial.println();
}

void Protocol::sendError(const String& message, ResponseStatus status) {
    JsonDocument response;
    response["status"] = status;
    response["error"] = message;
    
    serializeJson(response, Serial);
    Serial.println();
}

void Protocol::sendDebug(const String& message) {
    JsonDocument msg;
    msg["type"] = "debug";
    msg["timestamp"] = millis();
    msg["message"] = message;
    
    serializeJson(msg, Serial);
    Serial.println();
}

void Protocol::sendEvent(const String& eventType, const JsonDocument& data) {
    JsonDocument event;
    event["type"] = "event";
    event["event_type"] = eventType;
    event["timestamp"] = millis();
    event["data"] = data;
    
    serializeJson(event, Serial);
    Serial.println();
}
