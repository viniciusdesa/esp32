#include <Arduino.h>
#include "WiFi.h"

void setup()
{
    Serial.begin(115200);
    // Set ESP32 wifi to station mode
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    Serial.println("Setup done");
}
void loop()
{
    Serial.println("scan start");
    // WiFi.scanNetworks will give total found wifi networks
    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    
    if (n == 0)
    {
        Serial.println("no networks found");
    }
    else
    {
        Serial.print(n);
        Serial.println(" networks found");
        for (int i = 0; i < n; ++i)
        {
            // Print SSID and signal strength
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(")");
            Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
            delay(10);
        }
    }
    Serial.println("");
    // delay of 5 sec
    delay(5000);
}