#ifndef utils_h
#define utils_h

#include <ArduinoJson.h>
#include <FS.h>

// read file from SPIFFS
bool readFile(String path, String &buf)
{
    File f = SPIFFS.open(path, "r");

    if (!f)
        return false;

    while (f.available())
        buf += (char)f.read();

    f.close();

    return true;
}

// write file to SPIFFS
bool writeFile(String path, String &buf)
{
    File f = SPIFFS.open(path, "w+");

    if (!f)
        return false;

    uint32_t len = buf.length();

    for (uint32_t i = 0; i < len; i++)
        f.write(buf.charAt(i));
    f.close();

    return true;
}

/********************************************************************************/

JsonVariant parseJSONFile(String path, DynamicJsonBuffer &jsonBuffer)
{
    JsonVariant root;
    String buf = "";

    if (!readFile(path, buf))
    {
        buf = "{}";
    }

    root = jsonBuffer.parse(buf);

    return root;
}

String str(const char *ptr)
{
    char keyword[strlen_P(ptr)];
    strcpy_P(keyword, ptr);
    return String(keyword);
}

#endif