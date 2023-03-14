#include <Arduino.h>
#include <WiFi.h> /*Wifi library included*/

const char *ssid = "ESP32";         /*SSID defined for AP*/
const char *password = "12345678"; /*Password defined, removed for an open network*/
WiFiServer server(80);              /*Web Server Port 80*/

String header; /*variable stores HTTP request*/

String OutputGPIO25 = "OFF"; /*variable to store current Output*/
const int Output_25 = 25;    /*GPIO pin 25 assigned to variable*/

void setup()
{
    Serial.begin(115200);
    pinMode(Output_25, OUTPUT);   /*variable initialize for output*/
    digitalWrite(Output_25, LOW); /*Output set to low*/

    Serial.print("Setting AP (Access Point)…");
    WiFi.softAP(ssid, password); /*ESP32 wifi set in Access Point mode*/

    IPAddress IP = WiFi.softAPIP(); /*IP address is initialized*/
    Serial.print("AP IP address: ");
    Serial.println(IP); /*Print IP address*/
    server.begin();
}
void loop()
{
    WiFiClient client = server.available(); /*check for clients request*/

    if (client)
    { /*Condition to check for new client*/
        Serial.println("New Client.");
        String currentLine = ""; /*string to hold data*/
        while (client.connected())
        { /*loop for client connection check*/
            if (client.available())
            { /*read if data available*/
                char c = client.read();
                Serial.write(c);
                header += c;
                if (c == '\n')
                { /*if byte is newline character*/
                    /*in case if current line is blank two new line characters will be available*/
                    /*end of client hTTP request*/
                    if (currentLine.length() == 0)
                    {
                        Serial.println(currentLine);
                        break;
                    }
                    else
                    { /*in new line clear current line*/
                        currentLine = "";
                    }
                }
                else if (c != '\r')
                {                     /*a carriage return character*/
                    currentLine += c; /*add to the end of currentLine*/
                }
            }
        }
        /*clear header*/
        header = "";
        client.stop(); /*client disconnected*/
        Serial.println("Client disconnected.");
        Serial.println("");
    }
}