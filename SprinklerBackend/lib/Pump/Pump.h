#ifndef PUMP_H
#define PUMP_H

#include <Arduino.h>
#include <Preferences.h>

class Pump {
  private:
    int pin; 
    bool isInitialized;
    bool isActive;
    bool pumpNowFlag;
    unsigned long duration;
    unsigned long cycle;
    unsigned long previousMillis;
    unsigned long startTime;
    
  public:
    Pump(int pin);
    void pumpON(unsigned long currentMillis, Preferences& prefs);
    void pumpNow(unsigned long currentMillis, unsigned long duration);
    void update(unsigned long duration, unsigned long cycle, Preferences& prefs);
    void loadFromPrefs(Preferences& prefs);
    bool getIsActive();
    bool getIsInitialized();
    bool getPumpNowFlag();
    bool setPumpNowFlag(bool value);
    String getKey(const char* baseName);
};

#endif