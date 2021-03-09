/*===========================================================================
  Title: I2C Scanner
    - Repository: https://gist.github.com/AustinSaintAubin/dc8abb2d168f5f7c27d65bb4829ca870
  
  Version: 8
  Date: 2018 / 02 / 17
  Author: Austin St. Aubin
  Email: AustinSaintAubin@gmail.com
  
  Description: 
    Scans for i2c Devices
  
  Version History:
    Version 8
      Added preprocessor statments for display 
    Version 6
      Added i2c Ports
      Added OLED output
    Version 5, March 28, 2013
      As version 4, but address scans now to 127.
      A sensor seems to use address 120.
    Version 4, March 3, 2013, Using Arduino 1.0.3
      by Arduino.cc user Krodal.
      Changes by louarnold removed.
      Scanning addresses changed from 0...127 to 1...119,
      according to the i2c scanner by Nick Gammon
      http://www.gammon.com.au/forum/?id=10896
    Version 3, Feb 26  2013
      V3 by louarnold
    Version 2, Juni 2012, Using Arduino 1.0.1
       Adapted to be as simple as possible by Arduino.cc user Krodal
    Version 1
      This program (or code that looks like it)
      can be found in many places.
      For example on the Arduino.cc forum.
      The original author is not known.
      
  Notes:
  
    This sketch tests the standard 7-bit addresses
    Devices with higher bit address might not be seen properly.
    
  ============================================================================= */


#include <Arduino.h>
#include <Wire.h>

const uint8_t I2C_SDA_PIN = GPIO_NUM_33; //SDA;  // i2c SDA Pin
const uint8_t I2C_SCL_PIN = GPIO_NUM_32; //SCL;  // i2c SCL Pin

void setup()
{
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

    Serial.begin(115200);
    Serial.println("\nI2C Scanner");
}

void loop()
{
    byte error, address;
    int nDevices;

    Serial.println("Scanning...");

    nDevices = 0;
    for (address = 1; address < 127; address++)
    {
        // The i2c_scanner uses the return value of
        // the Write.endTransmisstion to see if
        // a device did acknowledge to the address.
        Wire.beginTransmission(address);
        error = Wire.endTransmission();

        if (error == 0)
        {
            nDevices++;

            Serial.print("Device found @ 0x");
            if (address < 16)
                Serial.print("0");
            Serial.print(address, HEX);
            Serial.println("  !");

        }
        else if (error == 4)
        {
            Serial.print("Unknow error @ 0x");
            if (address < 16)
                Serial.print("0");
            Serial.println(address, HEX);
        }
    }
    if (nDevices == 0)
    {
        Serial.println("No I2C devices found\n");
    }
    else
    {
        Serial.println("done\n");
    }

    // DEBUGING
    //  for(address = 40; address < 52; address++ ) {
    //    nDevices++;
    //    display.drawString(((nDevices -1) % 4) * 32, 23 + (round((nDevices -1) /4) *10), (String)nDevices + ":x" + String(address, HEX));
    //    Serial.println(address,HEX);
    //  }

    delay(5000); // wait 5 seconds for next scan
}