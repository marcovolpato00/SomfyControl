#include "Devices.h"

Devices::Devices()
{
    devicesList = new LinkedList<Device>();
}

void Devices::load()
{
    DynamicJsonBuffer jsonBuffer(512);
    JsonObject &root = parseJSONFile(DEVICES_FILE_PATH, jsonBuffer);
    JsonArray &devices = root["devices"];

    for (auto &_device : devices)
    {
        JsonObject &device = _device.as<JsonObject>();
        int button = device.get<int>(str(BUTTON_KEY));
        String name = device.get<String>(str(NAME_KEY));
        String description = device.get<String>(str(DESCRIPTION_KEY));

        Device d = {button, name, description};

        devicesList->add(d);
    }

    Serial.println("Devices loaded");
}

void Devices::save()
{
    String buf = serializeToJsonString();
    writeFile(DEVICES_FILE_PATH, buf);
}

String Devices::serializeToJsonString()
{
    DynamicJsonBuffer jsonBuffer(512);
    JsonArray &devices = jsonBuffer.createArray();

    int size = devicesList->size();
    for (int i = 0; i < size; i++)
    {
        Device d = devicesList->get(i);
        JsonObject &nested = devices.createNestedObject();

        nested[str(BUTTON_KEY)] = d.button;
        nested[str(NAME_KEY)] = d.name;
        nested[str(DESCRIPTION_KEY)] = d.description;
    }

    String buf;
    devices.printTo(buf);
    
    return buf;
}
