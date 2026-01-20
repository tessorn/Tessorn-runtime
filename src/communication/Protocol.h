#ifndef TESSORN_PROTOCOL_H
#define TESSORN_PROTOCOL_H

#include <Arduino.h>
#include <ArduinoJson.h>

enum CommandType {
    CMD_HANDSHAKE           = 0x01,
    CMD_GET_SENSOR_MAP      = 0x02,
    CMD_UPLOAD_BEHAVIOR     = 0x03,
    CMD_DELETE_BEHAVIOR     = 0x04,
    CMD_START_BEHAVIOR      = 0x05,
    CMD_STOP_BEHAVIOR       = 0x06,
    CMD_LIST_BEHAVIORS      = 0x07,
    CMD_GET_STATUS          = 0x08,
    CMD_VALIDATE_BEHAVIOR   = 0x09,
    CMD_DEBUG_ENABLE        = 0x0A,
    CMD_PING                = 0x0B,
    CMD_RESET               = 0x0C
};

enum ResponseStatus {
    STATUS_OK               = 0x00,
    STATUS_ERROR            = 0x01,
    STATUS_INVALID_COMMAND  = 0x02,
    STATUS_HARDWARE_ERROR   = 0x03,
    STATUS_VALIDATION_ERROR = 0x04,
    STATUS_STORAGE_ERROR    = 0x05
};

struct ValidationResult {
    bool passed;
    String errorMessage;
    String suggestion;
};

class Protocol {
public:
    void init();
    void processIncoming();
    
    static void sendSuccess(const JsonDocument& data);
    static void sendError(const String& message, ResponseStatus status = STATUS_ERROR);
    static void sendDebug(const String& message);
    static void sendEvent(const String& eventType, const JsonDocument& data);
    
private:
    String readCommand();
    void handleCommand(const JsonDocument& cmd);
    
    void handleHandshake(const JsonDocument& payload);
    void handleGetSensorMap(const JsonDocument& payload);
    void handleUploadBehavior(const JsonDocument& payload);
    void handleDeleteBehavior(const JsonDocument& payload);
    void handleStartBehavior(const JsonDocument& payload);
    void handleStopBehavior(const JsonDocument& payload);
    void handleListBehaviors(const JsonDocument& payload);
    void handleGetStatus(const JsonDocument& payload);
    void handleValidateBehavior(const JsonDocument& payload);
    void handlePing(const JsonDocument& payload);
    void handleReset(const JsonDocument& payload);
    
    bool debugEnabled = true;
};

#endif
