#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include "SomfyRemote.h"

#define HEARTBEAT_INTERVAL 300000 // 5 Minutes

uint64_t heartbeatTimestamp = 0;
bool isConnected = false;

WebSocketsClient webSocket;
SomfyRemote* _remote;
void webSocketEvent(WStype_t type, uint8_t *payload, size_t length);

void initSinric(SomfyRemote* remote, String apiKey)
{
    _remote = remote;

    // server address, port and URL
    webSocket.begin("iot.sinric.com", 80, "/");

    // event handler
    webSocket.onEvent(webSocketEvent);
    webSocket.setAuthorization("apikey", apiKey.c_str());

    webSocket.setReconnectInterval(5000); 
}

void runSinric()
{
    webSocket.loop();

    if (isConnected)
    {
        uint64_t now = millis();

        if ((now - heartbeatTimestamp) > HEARTBEAT_INTERVAL)
        {
            heartbeatTimestamp = now;
            webSocket.sendTXT("H");
        }
    }
}

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
        isConnected = false;
        Serial.printf("[WSc] Webservice disconnected from sinric.com!\n");
        break;
    case WStype_CONNECTED:
    {
        isConnected = true;
        Serial.printf("[WSc] Service connected to sinric.com at url: %s\n", payload);
        Serial.printf("Waiting for commands from sinric.com ...\n");
    }
    break;
    case WStype_TEXT:
    {
        Serial.printf("[WSc] get text: %s\n", payload);
        // Example payloads

        // On Off format https://developer.amazon.com/docs/device-apis/alexa-powercontroller.html
        // {"deviceId": xxxx, "action": "setPowerState", value: "ON"}

        // For Window Shutters https://developer.amazon.com/docs/device-apis/alexa-channelcontroller.html
        // {"deviceId":"5a90faedd923a349530330c3","action":"SkipChannels","value":{"channelCount":1}}
        // {"deviceId":"5a90faedd923a349530330c3","action":"ChangeChannel","value":{"channel":{"number":"200"},"channelMetadata":{}}}

        DynamicJsonBuffer jsonBuffer;
        JsonObject &json = jsonBuffer.parseObject((char *)payload);
        String deviceId = json["deviceId"];
        String action = json["action"];

        if (deviceId == "YOUR_DEVICE_ID") // Device ID of first device
        {
            if (action == "setPowerState")
            { // On or Off
                String value = json["value"];
                if (value == "ON")
                {
                    _remote->clickButton(2);
                }
                else
                {
                    _remote->clickButton(2);
                }
            }
            else if (action == "test")
            {
                Serial.println("[WSc] received test command from sinric.com");
            }
        }
    }

    break;
    case WStype_BIN:
        Serial.printf("[WSc] get binary length: %u\n", length);
        break;
    }
}
