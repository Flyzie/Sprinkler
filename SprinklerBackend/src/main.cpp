#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <esp_timer.h>
#include <Pump.h>
#include <vector>
#include <Preferences.h>
#include <ESPmDNS.h>
#include <config.h>

using namespace std;

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

const int PORT = SERVER_PORT;


AsyncWebServer server(PORT);

Preferences preferences;


#define PIN1 18
#define PIN2 19
#define WIFI_STANDBY_PIN 4
#define WIFI_READY_PIN 2

vector<Pump> pumps;


void setup() {
  Serial.begin(115200);

  preferences.begin("pumps-refs", false);

  pumps.push_back(Pump(PIN1));
  pumps.push_back(Pump(PIN2));

  for(Pump& pump : pumps){
    pump.loadFromPrefs(preferences);
  }
  
  pinMode(WIFI_STANDBY_PIN, OUTPUT);
  pinMode(WIFI_READY_PIN, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(WIFI_STANDBY_PIN, HIGH);
    delay(500);
    digitalWrite(WIFI_STANDBY_PIN, LOW);
    delay(500);
    Serial.println("Connecting to WiFi..");
  }

  if (WiFi.status() == WL_CONNECTED){
    Serial.print("Wifi connected! SSID:  " );
    digitalWrite(WIFI_READY_PIN, HIGH);
    Serial.println(ssid);
  }

  if (!MDNS.begin("sprinkler")) { 
    Serial.println("Error setting up MDNS responder!");
  } else {
    Serial.println("mDNS responder started");
    Serial.println("You can access this device at: http://sprinkler.local");
  }

  MDNS.addService("http", "tcp", 8080);

  Serial.print("ESP32 WebServer running at: http://");
  Serial.print(WiFi.localIP());
  Serial.print(":");
  Serial.println(PORT);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(200, "text/plain", "Hello from ESP32!");
  });

  static String body;

  server.on("/pumpUpdate", HTTP_POST, 
      [](AsyncWebServerRequest *request)
      {
        Serial.println("1");
      },
      [](AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final)
      {
        Serial.println("2");
      },
      [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
      {

        body += String((char*)data).substring(0, len);

        if (index + len == total) { 
            Serial.println("Full JSON body: " + body);

            JsonDocument doc; 
            DeserializationError error = deserializeJson(doc, body);

            if (error) {
                request->send(400, "text/plain", "Invalid JSON");
                body = "";
                return;
            }

            JsonArray data = doc["data"];

            Serial.println("JSON data: " + data);

            for(JsonObject pump : data){
              int pumpId = pump["pump"];
              int duration = pump["duration"];
              int cycle = pump["cycle"];

              if (pumpId < 0 || pumpId > 1 || duration < 0 || cycle < 0) {
                  request->send(400, "text/plain", "Invalid control inputs");
                  body = "";
                  return;
              }

              pumps[pumpId].update(duration, cycle, preferences);

              Serial.printf("Pump: %d, Duration: %d, Cycle: %d\n", pumpId, duration, cycle);
            }

            request->send(200, "text/plain", "Pump(s) updated!");
            body = ""; 
        }
      });

  server.onNotFound([](AsyncWebServerRequest *request) {
  if (request->method() == HTTP_OPTIONS) {
    request->send(200);
  } else {
    request->send(404, "text/plain", "Not found");
  }
  });

  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type");

  server.begin();
  Serial.println("Server started!");
}


void loop() {
  unsigned long currentMillis = millis();
  for(Pump& pump : pumps){
    if(pump.getIsInitialized()){
    pump.pumpON(currentMillis, preferences);
    }
  }
}
