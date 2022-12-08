#include <Arduino.h>

/**
 * @brief Conversion factor for micro seconds to seconds
 * 
 */
#define uS_TO_S_FACTOR 1000000
/**
 * @brief Time ESP32 will go to sleep (in seconds)
 * 
 */
#define TIME_TO_SLEEP 30        
/**
 * @brief Greater the value, more the sensitivity
 * 
 */
#define Threshold 20

/**
 * @brief Boot count
 * 
 */
RTC_DATA_ATTR int bootCount = 0;
/**
 * @brief Touch pin
 * 
 */
touch_pad_t touchPin;

/*
Method to print the touchpad by which ESP32
has been awaken from sleep
*/
void print_wakeup_touchpad()
{
    touchPin = esp_sleep_get_touchpad_wakeup_status();

    switch (touchPin)
    {
    case 0:
        Serial.println("Touch detected on GPIO 4");
        break;
    case 1:
        Serial.println("Touch detected on GPIO 0");
        break;
    case 2:
        Serial.println("Touch detected on GPIO 2");
        break;
    case 3:
        Serial.println("Touch detected on GPIO 15");
        break;
    case 4:
        Serial.println("Touch detected on GPIO 13");
        break;
    case 5:
        Serial.println("Touch detected on GPIO 12");
        break;
    case 6:
        Serial.println("Touch detected on GPIO 14");
        break;
    case 7:
        Serial.println("Touch detected on GPIO 27");
        break;
    case 8:
        Serial.println("Touch detected on GPIO 33");
        break;
    case 9:
        Serial.println("Touch detected on GPIO 32");
        break;
    default:
        Serial.println("Wakeup not by touchpad");
        break;
    }
}

/**
 * @brief Prints the reason by which ESP32 has been awaken from sleep.
 *
 */
void print_wakeup_reason()
{
    esp_sleep_wakeup_cause_t wakeup_reason;

    wakeup_reason = esp_sleep_get_wakeup_cause();

    switch (wakeup_reason)
    {
    case ESP_SLEEP_WAKEUP_EXT0:
        Serial.println("Wakeup caused by external signal using RTC_IO");
        break;
    case ESP_SLEEP_WAKEUP_EXT1:
        Serial.println("Wakeup caused by external signal using RTC_CNTL");
        break;
    case ESP_SLEEP_WAKEUP_TIMER:
        Serial.println("Wakeup caused by timer");
        break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD:
        Serial.println("Wakeup caused by touchpad");
        print_wakeup_touchpad();
        break;
    case ESP_SLEEP_WAKEUP_ULP:
        Serial.println("Wakeup caused by ULP program");
        break;
    default:
        Serial.printf("Wakeup was not caused by deep sleep: %d\n",
                      wakeup_reason);
        break;
    }
}

void callback()
{
    // placeholder callback function
}

void setup()
{
    Serial.begin(115200);
    delay(1000); // Take some time to open up the Serial Monitor

    // Increment boot number and print it every reboot
    ++bootCount;
    Serial.println("Boot number: " + String(bootCount));

    // Print the wakeup reason for ESP32
    print_wakeup_reason();

    // Setup interrupt on Touch Pad 9 (GPIO32)
    touchAttachInterrupt(T9, callback, Threshold);

    // First we configure the wake up source:
    // We set our ESP32 to wake up every 5 seconds
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) +
                   " Seconds");
    esp_sleep_enable_touchpad_wakeup();
    Serial.println("Setup ESP32 to wakeup from touchpad");

    Serial.println("Going to sleep now");
    delay(1000);
    Serial.flush();
    esp_deep_sleep_start();
    Serial.println("This will never be printed");
}

void loop()
{
}