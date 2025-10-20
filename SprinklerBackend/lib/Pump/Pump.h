#ifndef PUMP_H
#define PUMP_H

#include <Arduino.h>
#include <Preferences.h>

class Pump {
  private:
    int pin; 
    bool isInitialized;
    bool isActive;
    unsigned long duration;
    unsigned long cycle;
    unsigned long previousMillis;
    unsigned long startTime;
    
  public:
    Pump(int pin);
    void pumpON(unsigned long currentMillis, Preferences& prefs);
    void update(unsigned long duration, unsigned long cycle, Preferences& prefs);
    void loadFromPrefs(Preferences& prefs);
    bool getIsActive();
    bool getIsInitialized();
    String getKey(const char* baseName);
};

#endif