#include <Arduino.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "config.h"

#define mqtt_server "192.168.43.43"

const int PIN_SOIL_HUMIDITY = A0;
const int PIN_LED = D1;
const int PIN_LED_R = D2;
const int PIN_LED_G = D3;
const int PIN_LED_B = D4;
const int PIN_DHT = D5;
const int DHT_TYPE = DHT11;

const int MINIMAL_HUMIDITY = 45;
int soilHumidity = 0;
int soilHumidityPercent = 0;

DHT dht(PIN_DHT, DHT_TYPE);
float dhtHumidity = 0;
float dhtTemperature = 0;

int millisHumidity = 0;
int millisConsole = 0;
int millisHomeAssistant = 0;

WiFiClient espClient;
PubSubClient client(espClient);

const char *TOPIC_TEMPERATURE = "garden/greenhouse/temperature";
const char *TOPIC_HUMIDITY = "garden/greenhouse/humidity";
const char *TOPIC_SOIL_HUMIDITY = "garden/greenhouse/soil_humidity";

void printDHTValues(float h, float t, float hic)
{
    Serial.println("-----------");
    Serial.print("> Temperature: ");
    Serial.println(t);
    Serial.print("> Humidity: ");
    Serial.println(h);
    Serial.print("> Heat index: ");
    Serial.println(hic);
    Serial.println("-----------");
}

void setup()
{
    Serial.begin(9600);
    dht.begin();

    WiFi.begin(WIFI_SSID, WIFI_PASS);

    Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println();

    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());

    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, LOW);

    pinMode(PIN_LED_R, OUTPUT);
    analogWrite(PIN_LED_R, 0);

    pinMode(PIN_LED_G, OUTPUT);
    analogWrite(PIN_LED_G, 0);

    pinMode(PIN_LED_B, OUTPUT);
    analogWrite(PIN_LED_B, 0);

    client.setServer(mqtt_server, 1883);
}

void loop()
{
    if (millis() - millisHumidity > 300)
    {
        soilHumidity = analogRead(PIN_SOIL_HUMIDITY);
        analogWrite(PIN_LED_R, soilHumidity);
        analogWrite(PIN_LED_G, 1024 - soilHumidity);

        soilHumidityPercent = map(soilHumidity, 0, 1024, 100, 0);
        Serial.print("Humidity: ");
        Serial.println(soilHumidityPercent);

        millisHumidity = millis();
    }

    if (millis() - millisConsole > 1500)
    {
        dhtHumidity = dht.readHumidity();
        dhtTemperature = dht.readTemperature();

        if (isnan(dhtHumidity) || isnan(dhtTemperature))
        {
            Serial.println("-----------");
            Serial.println("Failed to read from DHT sensor!");
            Serial.println("-----------");
        }
        else
        {
            float hic = dht.computeHeatIndex(dhtTemperature, dhtHumidity, false);
            printDHTValues(dhtHumidity, dhtTemperature, hic);
        }

        millisConsole = millis();
    }

    if (millis() - millisHomeAssistant > 5000)
    {
        client.publish(TOPIC_TEMPERATURE, String(dhtTemperature).c_str(), true);
        client.publish(TOPIC_HUMIDITY, String(dhtHumidity).c_str(), true);
        client.publish(TOPIC_SOIL_HUMIDITY, String(soilHumidity).c_str(), true);
        Serial.println("-----------");
        Serial.println("Home Assistant updated!");
        Serial.println("-----------");

        millisHomeAssistant = millis();
    }
}