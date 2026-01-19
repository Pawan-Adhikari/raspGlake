#include "HumidityS.hpp"

HumidityData getHumidity(int aPin, int dPin, bool scale){
    HumidityData hd;
    int digitalValue = digitalRead(dPin);
    float analogValue = analogRead(aPin);
    if (!scale){
        hd.moisture = analogValue;
    }
    else{
        hd.moisture = (RAW_MAX_DRY - analogValue) / (RAW_MAX_DRY - RAW_MIN_WET) * 100.0;
    }
    hd.isDry = !digitalValue;
    return hd;
}   

 void HumidityData::display() const{
        Serial.println("Relative Humidity: ");
        Serial.println(moisture);
        Serial.println("Wet?");
        Serial.println(isDry);
    }