#include <Arduino.h>

const int PIN_SOIL_HUMIDITY = A0;
const int PIN_LED = D1;

const int PIN_LED_R = D2;
const int PIN_LED_G = D3;
const int PIN_LED_B = D4;

const int MINIMAL_HUMIDITY = 45;
int soilHumidity = 0;
int soilHumidityPercent = 0;

void setup()
{
    Serial.begin(9600);

    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, LOW);

    pinMode(PIN_LED_R, OUTPUT);
    analogWrite(PIN_LED_R, 0);

    pinMode(PIN_LED_G, OUTPUT);
    analogWrite(PIN_LED_G, 0);

    pinMode(PIN_LED_B, OUTPUT);
    analogWrite(PIN_LED_B, 0);
}

void loop()
{
    soilHumidity = analogRead(PIN_SOIL_HUMIDITY);
    analogWrite(PIN_LED_R, soilHumidity);
    analogWrite(PIN_LED_G, 1024 - soilHumidity);

    soilHumidityPercent = map(soilHumidity, 0, 1024, 100, 0);
    Serial.print("Humidity: ");
    Serial.println(soilHumidityPercent);
    delay(250);
}