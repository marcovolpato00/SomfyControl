#include "Auth.h"

Auth::Auth()
{
    generateCookie();
}

void Auth::init(ESP8266WebServer *server, String user, String pass)
{
    _server = server;
    username = user;
    password = pass;
    
}

bool Auth::is_authentified()
{
    if (_server->hasHeader("Cookie"))
    {
        String cookie = _server->header("Cookie");
        if (cookie.indexOf("c=" + sessionCookie) != -1)
            return true;
    }
    return false;
}

bool Auth::login()
{
    if (_server->hasArg("user") && _server->hasArg("pass"))
    {
        if (_server->arg("user") == username && _server->arg("pass") == password)
        {
            Serial.println("OK");
            _server->sendHeader("Set-Cookie", "c=" + sessionCookie);
            return true;
        }
    }
    return false;
}

void Auth::logout()
{
    generateCookie();
    _server->sendHeader("Set-Cookie", "c=0");
}

void Auth::generateCookie()
{
    sessionCookie = "";

    for (int i = 0; i < 32; i++)
        sessionCookie += chars[random(0, chars.length())];
}

// getters
String Auth::getPassword()
{
    return password;
}

String Auth::getUsername()
{
    return username;
}

// setters
void Auth::setUsername(String user)
{
    username = user;
}

void Auth::setPassword(String pass)
{
    password = pass;
}
