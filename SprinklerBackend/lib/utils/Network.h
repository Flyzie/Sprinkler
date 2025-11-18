#ifndef NETWORK_H
#define NETWORK_H

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>

class Network {
    private:
        const char* ssid;
        const char* password;
        const int port;
    public:
        Network();
        AsyncWebServer* initNetwork();
        bool checkClients();

};

#endif