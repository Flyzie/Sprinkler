#include <Pump.h>


Pump::Pump(int pin){
    this->pin = pin;
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    isInitialized = false;
    isActive = false;
    pumpNowFlag = false;
    duration = 0;
    pumpNowDuration = 0;
    cycle = 0;
    previousMillis = 0;
    startTime = 0;
}

void Pump::pumpON(unsigned long currentMillis, Preferences& prefs){
    if(!this->isActive && (currentMillis - this->previousMillis >= this->cycle)){
        this->isActive = true;
        this->startTime = currentMillis;
        digitalWrite(this->pin, HIGH);
        Serial.println("Pump " + String(this->pin) + " turned ON " + "at: " + millis() + " current millis: " + currentMillis);
    }

    if(this->isActive && (currentMillis - this->startTime >= this->duration)){
        this->isActive = false; 
        this->previousMillis = currentMillis;
        digitalWrite(this->pin, LOW);
        Serial.println("Pump " + String(this->pin) + " turned OFF " + "at: " + millis());
    }
}

void Pump::pumpNow(unsigned long currentMillis){
    if(!isActive){
        this->isActive = true;
        this->startTime = currentMillis;
        digitalWrite(this->pin, HIGH);
        Serial.println("Pump " + String(this->pin) + " turned ON " + "at: " + millis());
    }

    if(this->isActive && (currentMillis - this->startTime >= this->pumpNowDuration)){
        this->isActive = false; 
        digitalWrite(this->pin, LOW);
        this->pumpNowDuration = 0;
        this->pumpNowFlag = false;
        Serial.println("Pump " + String(this->pin) + " turned OFF " + "at: " + millis());
    }
}

void Pump::update(unsigned long duration, unsigned long cycle, Preferences& prefs, unsigned long currentMillis){
    this->isInitialized = true;
    this->duration = duration;
    this->cycle = cycle;
    this->previousMillis = currentMillis;

    prefs.putULong(getKey("duration").c_str(), duration);
    prefs.putULong(getKey("cycle").c_str(), cycle);

    Serial.println("Pump updated, previous millis:" + String(this->previousMillis) + "at: " + millis());
}

void Pump::reset(Preferences& prefs){
    this->isInitialized = false;
    this->duration = 0;
    this->cycle = 0;
    
    prefs.putULong(getKey("duration").c_str(), 0);
    prefs.putULong(getKey("cycle").c_str(), 0);
}

void Pump::loadFromPrefs(Preferences& prefs){
    this->duration = prefs.getULong(getKey("duration").c_str(), 0);
    this->cycle = prefs.getULong(getKey("cycle").c_str(), 0);

    if(duration > 0 && cycle > 0){
        this->isInitialized = true;
        Serial.printf("Loaded pump %d preferences: duration=%lu, cycle=%lu\n", this->pin, this->duration, this->cycle);
    } else {
        Serial.printf("No saved preferences for pump %d\n", this->pin);
    }
    
}

bool Pump::getIsActive(){
    return this->isActive;
    
}

bool Pump::getIsInitialized(){
    return this->isInitialized;
}

bool Pump::getPumpNowFlag(){
    return this->pumpNowFlag;
}

void Pump::setPumpNowFlag(bool value, unsigned int duration){
    this->pumpNowDuration = duration;
    this->pumpNowFlag = value;
}

String Pump::getKey(const char* baseName){
    return String(baseName) + String(this->pin);
}