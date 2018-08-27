#ifndef devices_h
#define devices_h

#include "Arduino.h"
#include <ArduinoJson.h>
#include <LinkedList.h>
#include "definitions.h"

extern JsonVariant parseJSONFile(String path, DynamicJsonBuffer &jsonBuffer);
extern bool writeFile(String path, String &buf);
extern String str(const char *ptr);

class Devices
{
public:
  Devices();
  void load();
  void save();
  String serializeToJsonString();

private:
  struct Device
  {
    int button;
    String name;
    String description;
  };
  LinkedList<Device> *devicesList;
};

#endif