#include <Arduino.h>
#include "INA219.h"
#include "Wire.h"

INA219 INA(0x40, &Wire);

void setup()
{
    Serial.begin(115200);
    Serial.println(__FILE__);

    //Serial.println("\nI2C Scanner");
    
    Wire.begin(33, 32);

    Serial.print("INA219_LIB_VERSION: ");
    Serial.println(INA219_LIB_VERSION);

    if (!INA.begin())
    {
        Serial.println("could not connect. Fix and Reboot");
        while(1);
    }

    INA.setBusVoltageRange(16);

    // 16 ou 32
    Serial.println("\nReading bus voltage range...");
    uint8_t busVoltageRange = INA.getBusVoltageRange();
    Serial.printf("Bus Voltage Range: %d\n", busVoltageRange);

    Serial.println("\nReading gain...");
    uint8_t gain = INA.getGain();
    Serial.printf("Gain: %d\n", gain);

    Serial.println("\nReading Bus ADC...");
    uint8_t busADC = INA.getBusADC();
    Serial.printf("Bus ADC: %d\n", busADC);

    Serial.println("\nReading Shunt ADC...");
    uint8_t shuntADC = INA.getShuntADC();
    Serial.printf("Shunt ADC: %d\n", shuntADC);

    Serial.println("\nReading Operating Mode...");
    uint8_t mode = INA.getMode();
    Serial.printf("Mode: %d\n", mode);

    Serial.println("Calibrating...");
    INA.setMaxCurrentShunt(5, 0.001);
    delay(1000);

    Serial.println("\nGetting current LSB...");
    float currentLSB = INA.getCurrentLSB();
    Serial.printf("CurrentLSB: %.4f\n", currentLSB);

    Serial.println("\nGetting shunt...");
    float shunt = INA.getShunt();
    Serial.printf("Shunt: %.4f\n", shunt);

    Serial.println("\nMax Current...");
    float maxCurrent = INA.getMaxCurrent();
    Serial.printf("Max Current: %.4f\n", maxCurrent);
}

void loop()
{
    Serial.println("    BUS     SHUNT       CURRENT     POWER");
    for (int i = 0; i < 20; i++)
    {
        Serial.print(INA.getBusVoltage(), 3);
        Serial.print("  |  ");
        Serial.print(INA.getShuntVoltage_mV(), 3);
        Serial.print("  |  ");
        Serial.print(INA.getCurrent_mA(), 3);
        Serial.print("  |  ");
        Serial.print(INA.getPower_mW(), 3);
        Serial.println();
        delay(1000);
    }
    delay(10000);
}