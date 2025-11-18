#include <Network.h>
#include <config.h>

Network::Network():ssid(WIFI_SSID), password(WIFI_PASSWORD), port(SERVER_PORT){
}

AsyncWebServer* Network::initNetwork(){
    WiFi.mode(WIFI_AP);
    WiFi.softAP(this->ssid, this->password);
    
    if (!MDNS.begin(MDNS_HOSTNAME)) { 
    Serial.println("Error setting up MDNS responder!");
    } else {
        Serial.println("mDNS responder started");
        Serial.println("You can access this device at: http://sprinkler.local");
    }

    MDNS.addService("http", "tcp", 8080);

    Serial.print("ESP32 WebServer running at: http://");
    Serial.print(WiFi.softAPIP());
    Serial.print(":");
    Serial.println(this->port);

    return new AsyncWebServer (this->port);
}

bool Network::checkClients(){
    int clients = WiFi.softAPgetStationNum();

    if (clients > 0){
        return true;
    }else{
        return false;
    }
}