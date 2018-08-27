#include "Settings.h"

Settings::Settings()
{
}

void Settings::load()
{
    DynamicJsonBuffer jsonBuffer(512);
    JsonObject &config = parseJSONFile(CONFIG_FILE_PATH, jsonBuffer);

    /*if (config.containsKey(str(SSID_KEY)))
        ssid = config.get<String>(str(SSID_KEY));
    if (config.containsKey(str(PASSWORD_KEY)))
        password = config.get<String>(str(PASSWORD_KEY));
    if (config.containsKey(str(SINRIC_ENABLED_KEY)))
        sinricEnabled = config.get<bool>(str(SINRIC_ENABLED_KEY));
    if (config.containsKey(SINRIC_KEY))
        sinricApiKey = config.get<String>(str(SINRIC_KEY));*/

    ssid = config.get<String>(str(SSID_KEY));
    password = config.get<String>(str(PASSWORD_KEY));
    sinricEnabled = config.get<bool>(str(SINRIC_ENABLED_KEY));
    sinricApiKey = config.get<String>(str(SINRIC_KEY));

    Serial.println("Settings loaded");
}

bool Settings::save()
{
    String buf = serializeToJsonString();
    bool success = writeFile(CONFIG_FILE_PATH, buf);
    return success;
}

bool Settings::update(String json)
{
    DynamicJsonBuffer jsonBuffer(512);
    JsonObject &config = jsonBuffer.parseObject(json);

    if(!config.success())
        return false;

    config.printTo(json);
    bool success = writeFile(CONFIG_FILE_PATH, json); 

    if(success)
        load();
    
    return success;
}

String Settings::serializeToJsonString()
{
    DynamicJsonBuffer jsonBuffer(512);
    JsonObject &config = jsonBuffer.createObject();

    config.set(SSID_KEY, ssid);
    config.set(PASSWORD_KEY, password);
    config.set(SINRIC_ENABLED_KEY, sinricEnabled);
    config.set(SINRIC_KEY, sinricApiKey);

    String buf;
    config.printTo(buf);

    return buf;
}

// getters
String Settings::getSSID()
{
    return ssid;
}

String Settings::getPassword()
{
    return password;
}

bool Settings::getSinricEnabled()
{
    return sinricEnabled;
}

String Settings::getSinricApiKey()
{
    return sinricApiKey;
}

// setters
void Settings::setSSID(String ssid)
{
    Settings::ssid = ssid;
}

void Settings::setPassword(String password)
{
    Settings::password = password;
}

void Settings::setSinricEnabled(bool enabled)
{
    sinricEnabled = enabled;
}

void Settings::setSinricApiKey(String key)
{
    sinricApiKey = key;
}
