/*

  ___             __       ___         _           _ 
 / __| ___ _ __  / _|_  _ / __|___ _ _| |_ _ _ ___| |
 \__ \/ _ \ '  \|  _| || | (__/ _ \ ' \  _| '_/ _ \ |
 |___/\___/_|_|_|_|  \_, |\___\___/_||_\__|_| \___/_|
                     |__/                            

  Copyright 2018 Marco Volpato

*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>

#include "webfiles.h"
#include "sinric.h"
#include "utils.h"
#include "SomfyRemote.h"
#include "Settings.h"
#include "Auth.h"

ESP8266WebServer server(80);
SomfyRemote remote;
Settings settings;
Auth auth;

void setup()
{
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // init SPIFFS
  if (SPIFFS.begin())
  {
    Serial.println("SPIFFS Init OK");
  }
  else
  {
    Serial.println("SPIFFS Init Failed");
  }

  settings.load();

  // init WiFi
  WiFi.begin(settings.getSSID().c_str(), settings.getPassword().c_str());
  Serial.print("Connecting to: ");
  Serial.println(settings.getSSID());
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  auth.init(&server, settings.getWebUsername(), settings.getWebPassword());
  
  startWebServer();

  if (settings.getSinricEnabled())
    initSinric(&remote, settings.getSinricApiKey());
}

void loop()
{
  server.handleClient();

  if (settings.getSinricEnabled())
    runSinric();
}

/********************************************************************************************/

void startWebServer()
{
  server.on("/", handleIndex);

  server.on("/index.html", handleIndex);

  server.on("/login.html", []() {
    sendProgmem(loginPage, sizeof(loginPage), "text/html");
  });

  server.on("/main.js", []() {
    sendProgmem(mainScript, sizeof(mainScript), "application/javascript");
  });

  server.on("/manifest.json", []() {
    sendProgmem(manifestJson, sizeof(manifestJson), "application/json");
  });

  server.on("/translate.js", []() {
    sendProgmem(translateScript, sizeof(translateScript), "application/javascript");
  });

  server.on("/langs/fr.lang", []() {
    sendProgmem(frLang, sizeof(frLang), "application/json");
  });

  server.on("/langs/it.lang", []() {
    sendProgmem(itLang, sizeof(itLang), "application/json");
  });

  server.on("/login", handleLogin);
  server.on("/logout", handleLogout);
  server.on("/command", handleCommands);
  server.on("/devices.json", handleDevices);
  server.on("/config.json", handleConfig);

  const char *headerKeys[] = {"Cookie"};
  size_t headerKeysSize = sizeof(headerKeys) / sizeof(char *);
  // ask server to track these headers
  server.collectHeaders(headerKeys, headerKeysSize);

  server.begin();
  Serial.println("Web server started");
}

void sendProgmem(const char *ptr, size_t size, const char *type)
{
  server.sendHeader("Content-Encoding", "gzip");
  /*if (type != "application/json")
    server.sendHeader("Cache-Control", "max-age=86400");*/
  server.sendHeader("Cache-Control", "no-cache");
  server.send_P(200, str(type).c_str(), ptr, size);
}

void handleIndex()
{
  if (!auth.is_authentified())
    redirect("/login.html");
  else
    sendProgmem(indexPage, sizeof(indexPage), "text/html");
}

void handleDevices()
{
  if (!auth.is_authentified())
    redirect("/login.html");
  else
    handleJson("/devices.json");
}

void handleConfig()
{
  if (!auth.is_authentified())
    redirect("/login.html");
  else
    handleJson("/config.json");
}

void handleJson(String path)
{
  String json;
  if (readFile(path, json))
    server.send(200, "application/json", json);
  else
    server.send(200, "application/json", "{}");
}

void handleCommands()
{
  if (!auth.is_authentified())
    redirect("/login.html");
  else
  {
    String action = server.arg("action");
    bool success;

    if (action == "click")
    {
      int button = server.arg("button").toInt();
      if (button != 16)
        success = remote.clickButton(button);
      else
      {
        success = true;
        digitalWrite(LED_BUILTIN, LOW);
        delay(500);
        digitalWrite(LED_BUILTIN, HIGH);
      }
    }
    else if (action == "program")
    {
      success = true;
      /*int button = server.arg("button").toInt();
    success = programNewButton(button);*/
    }
    else if (action == "updateSettings")
    {
      String config = server.arg("plain");
      success = settings.update(config);
      if (success)
        ESP.reset();
    }

    if (success)
    {
      server.send(200, "text/plain", "OK");
    }
    else
    {
      server.send(500, "text/plain", "Failed");
    }
  }
}

void handleLogin()
{
  if (auth.login())
    redirect("/");
  else
    redirect("/login.html");
}

void handleLogout()
{
  auth.logout();
  redirect("/login.html");
}

void redirect(String location)
{
  server.sendHeader("Location", location);
  server.sendHeader("Cache-Control", "no-cache");
  server.send(301);
}
