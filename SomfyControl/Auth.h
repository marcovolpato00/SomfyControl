#ifndef auth_h
#define auth_h

#include "Arduino.h"
#include <ESP8266WebServer.h>

class Auth {
    public:
        Auth();
        void init(ESP8266WebServer* server, String user, String pass);
        bool is_authentified();
        bool login();
        void logout();

        void setUsername(String user);
        void setPassword(String pass);
        String getUsername();
        String getPassword();

    private:
        ESP8266WebServer* _server;
        String username = "";
        String password = "";
        String chars = "abcdefghijklmnopqrstuvwxyz0123456789";
        String sessionCookie = "";
        int wrongLoginAttemps = 0;

        void generateCookie();
};

#endif