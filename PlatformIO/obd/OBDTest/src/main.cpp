#include <Arduino.h>
#include "GPSHandler.h"
#include "LogManager.h"

void setup() 
{
    Serial.begin(115200);

    Serial.printf("[setup] Iniciando armazenamento...\n");

    if (!StorageController.begin(GPIO_NUM_5))
    {
        Serial.printf("[setup] Falha ao iniciar armazenamento.\n");
        while(1);        
    }

    GPSSettings_t gpsSettings;
    gpsSettings.Model = 0;

    Serial.printf("[setup] Iniciando GPS...\n");
    GPS.begin(GPIO_NUM_22, GPIO_NUM_21, gpsSettings);

    Serial.printf("[setup] Iniciando gerenciador do log...\n");
    LogManager.begin();

    Serial.printf("[setup] Sistema iniciado...\n");
}

void loop() 
{
    GPSContext_t data = GPS.getData();

    if (data.IsValid)
    {
        Serial.printf("[loop] GPS: %s\n", GPS.toString(data).c_str());
        LogManager.logData(GPS.toString(data).c_str());
    }

    delay(10);
}