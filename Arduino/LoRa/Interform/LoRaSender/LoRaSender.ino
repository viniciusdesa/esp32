/* 
  Basic test program, send date at the BAND you seted.
  
  by Aaron.Lee from HelTec AutoMation, ChengDu, China
  成都惠利特自动化科技有限公司
  www.heltec.cn
  
  this project also realess in GitHub:
  https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series
*/
#include <SPI.h>
#include <LoRa.h>

// Pin definetion of WIFI LoRa 32
// HelTec AutoMation 2017 support@heltec.cn 
//#define SCK     5    // GPIO5  -- SX127x's SCK
//#define MISO    19   // GPIO19 -- SX127x's MISO
//#define MOSI    27   // GPIO27 -- SX127x's MOSI
//#define SS      18   // GPIO18 -- SX127x's CS
//#define RST     14   // GPIO14 -- SX127x's RESET
//#define DI0     26   // GPIO26 -- SX127x's IRQ(Interrupt Request)

#define SCK       14   // GPIO14  -- SX127x's SCK
#define MISO      13   // GPIO13 -- SX127x's MISO
#define MOSI      12   // GPIO12 -- SX127x's MOSI
#define SS        15   // GPIO15 -- SX127x's CS
#define RST       32   // GPIO32 -- SX127x's RESET
#define DI0       18   // GPIO26 -- SX127x's IRQ(Interrupt Request)

#define BAND    433E6  //you can set band here directly,e.g. 433E6,868E6,915E6
#define PABOOST true

#define GPIO_LED  27

int counter = 0;

void setup() 
{
    pinMode( GPIO_LED, OUTPUT );
    
    Serial.begin( 115200 );
    while ( !Serial );
    Serial.println( "LoRa Sender" );
    
    SPI.begin( SCK, MISO, MOSI, SS );
    LoRa.setPins( SS, RST, DI0 );
    
    if (!LoRa.begin( BAND, PABOOST ))
    {
        Serial.println("Starting LoRa failed!");
        while (1);
    }
}

void loop() 
{
    digitalWrite( GPIO_LED, HIGH );   // turn the LED on (HIGH is the voltage level)
    delay( 1000 );                       // wait for a second
    
    Serial.print( "Sending packet: " );
    Serial.println( counter );
    
    // send packet
    LoRa.beginPacket();
    LoRa.print( "hello " );
    LoRa.print( counter );
    LoRa.endPacket();
    
    counter++;
    
    digitalWrite( GPIO_LED, LOW );    // turn the LED off by making the voltage LOW
    delay( 1000 );                       // wait for a second
}
