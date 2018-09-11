#include <Arduino.h>

const int PIN_SOIL_HUMIDITY = A0;
const int PIN_LED = D1;

const int MINIMAL_HUMIDITY = 45;
int soilHumidity = 0;

void setup()
{
    Serial.begin(9600);
    
    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, LOW);
}

void loop()
{
    soilHumidity = analogRead(PIN_SOIL_HUMIDITY);
    soilHumidity = map(soilHumidity, 0, 1024, 100, 0);
    if(soilHumidity < MINIMAL_HUMIDITY){
        digitalWrite(PIN_LED, HIGH);
    } else {
        digitalWrite(PIN_LED, LOW);
    }
    Serial.print("Humidity: ");
    Serial.println(soilHumidity);
    delay(500);
}
