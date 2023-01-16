#include <WiFi.h>
// Replace with your network credentials
const char* ssid     = "Matrix310";
const char* password = "ap-password";

void setup()
{
    Serial.begin(115200);
    delay(10);

    Serial.println();
    Serial.println();
    Serial.print("AP name: ");
    Serial.println(ssid);
    
    // Set Wifi mode
    WiFi.mode(WIFI_AP);
    // Remove the password parameter, if you want the AP (Access Point) to be open
    IPAddress IP = WiFi.softAP(ssid, password);
    Serial.print("AP IP address: ");
    Serial.println(IP);
}

void loop()
{
  
}

