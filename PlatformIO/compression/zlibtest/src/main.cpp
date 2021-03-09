#include <Arduino.h>
#include "zlib.h"
#include "CommonStorage.h"

unsigned char match[65280 + 2];         /* buffer for reversed match or gzip 32K sliding window */

/* Process the gun command line arguments.  See the command syntax near the
   beginning of this source file. */
void setup()
{
    Serial.begin(115200);

    if (!StorageController.begin(GPIO_NUM_5))
    {
        ESP_LOGE("", "Falha ao inicializar o microSD.");
        while(1);
    }

    

    while(1);    
}

void loop()
{
}

