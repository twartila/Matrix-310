/*
Using WIFI_STA to connect the WiFi.
Source: https://randomnerdtutorials.com/esp32-useful-wi-fi-functions-arduino/
*/
#include <WiFi.h>

const char* ssid     = "WiFi-ssid";
const char* password = "WiFi-password";

void setup()
{
    Serial.begin(115200);
    delay(10);

    // We start by connecting to a WiFi network
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    // Set Wifi mode
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void loop()
{
  
}

