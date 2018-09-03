#ifndef definitions_h
#define definitions_h

// config json
const char CONFIG_FILE_PATH[] PROGMEM = "/config.json";
const char SSID_KEY[] PROGMEM = "ssid";
const char PASSWORD_KEY[] PROGMEM = "password";
const char SINRIC_ENABLED_KEY[] PROGMEM = "sinric_enabled";
const char SINRIC_KEY[] PROGMEM = "sinric_api_key";
const char WEB_USER_KEY[] PROGMEM = "web_username";
const char WEB_PASS_KEY[] PROGMEM = "web_password";

// devices json
const char DEVICES_FILE_PATH[] PROGMEM = "/devices.json";
const char BUTTON_KEY[] PROGMEM = "button";
const char NAME_KEY[] PROGMEM = "name";
const char DESCRIPTION_KEY[] PROGMEM = "description";

#endif