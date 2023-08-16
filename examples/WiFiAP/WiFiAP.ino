/*
Using WIFI_AP mode makes Matrix310 to be a access point.
Source: https://randomnerdtutorials.com/esp32-useful-wi-fi-functions-arduino/
*/
#include <WiFi.h>
// Replace with your network credentials
const char* ssid     = "Matrix-310";
const char* password = "ap-password";

void setup()
{
    Serial.begin(115200);
    delay(10);

    Serial.println();
    Serial.println();
    Serial.print("AP name: ");
    Serial.println(ssid);
    Serial.print("AP password: ");
    Serial.println(password);
    
    // Set Wifi mode
    WiFi.mode(WIFI_AP);
    // Set the password parameter to NULL, if you want the AP (Access Point) to be open.
    // If you don't want to use the default IP, you can manually configure access point with softAPConfig(ip, gateway, subnet) before using WiFi.softAP to turn on the wifi ap.
    // Our example use default AP IP 192.168.4.1
    Serial.println(WiFi.softAP(ssid, password) ? "AP Ready" : "AP Failed!");
    Serial.print("AP IP address: ");
    Serial.println(WiFi.softAPIP());
}

void loop(){
  
}

