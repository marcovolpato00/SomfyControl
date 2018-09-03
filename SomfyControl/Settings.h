#ifndef settings_h
#define settings_h

#include "Arduino.h"
#include <ArduinoJson.h>
#include "definitions.h"
#include <FS.h>

extern JsonVariant parseJSONFile(String path, DynamicJsonBuffer &jsonBuffer);
extern bool writeFile(String path, String &buf);
extern String str(const char *ptr);

class Settings
{
public:
  Settings();
  void load();
  bool save();
  bool update(String json);

  // getters
  String getSSID();
  String getPassword();
  bool getSinricEnabled();
  String getSinricApiKey();
  String getWebUsername();
  String getWebPassword();

  //setters
  void setSSID(String ssid);
  void setPassword(String password);
  void setSinricEnabled(bool enabled);
  void setSinricApiKey(String key);
  void setWebUsername(String user);
  void setWebPassword(String pass);

private:
  String ssid = "";
  String password = "";
  bool sinricEnabled = false;
  String sinricApiKey = "";
  String webUsername = "";
  String webPassword = "";

  String serializeToJsonString();
};

#endif
