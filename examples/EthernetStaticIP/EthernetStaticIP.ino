/*
Using Ethernet.begin(mac, ip) with the proper network setup, the Ethernet shield will configure using static IP address instead of DHCP.
https://www.arduino.cc/reference/en/libraries/ethernet/
*/
#include <Ethernet.h>
#include "./src/Artila-Matrix310.h"

// The MAC (Media access control) address for the device (array of 6 bytes). 
// This is the Ethernet hardware address which is written on a sticker under your Matrix310.
byte mac[] = { 0x98, 0xf4, 0xab, 0x17, 0x24, 0xc6 };
// the IP address of the device (array of 4 bytes)
byte ip[] = {192, 168, 1, 125};

void setup() {

  // Ethernet LAN chip select
  Ethernet.init(LAN_CS); // pin 5
  Serial.begin(115200);

  // start the Ethernet connection:
  Serial.println("Initialize Ethernet with static IP:");
  //Matrix310 tries connecting the internet with static ip
  Ethernet.begin(mac, ip);
  // Matrix-310 already connect to the internet
  Serial.print("  Static IP ");
  Serial.println(Ethernet.localIP());
  
  // give the Ethernet shield a second to initialize:
  delay(1000);
}

void loop() {
}
