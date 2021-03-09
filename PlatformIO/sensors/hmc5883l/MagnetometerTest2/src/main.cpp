#include <Arduino.h>
#include <Wire.h>
#include "QMC5883L.h"

void setup()
{
    Wire.begin(33, 32);
    Serial.begin(256000);

    Serial.println("Inicializando o sistema...");
    if (!QMC5883L.begin())
    {
        Serial.println("Falha ao configurar o sistema...");
        while(1);
    }

    //qmc.setMode(Mode_Continuous,QMC_ODR_200Hz,RNG_2G,OSR_256);
}

void loop()
{
    if (QMC5883L.ready())
    {
        int16_t x, y, z;
        int heading = 0;
        if (QMC5883L.read(x, y, z, heading) != 0)
            Serial.printf("%lu,%05d,%05d,%05d,%d\n", millis(), x, y, z, heading);

        // int heading = QMC5883L.readHeading();
        // if(heading != 0) 
        //     Serial.printf("[%lu] Heading: %d\n", millis(), heading);        
    }

    delay(1);
}