/*
Using Ethernet.begin(mac) with the proper network setup, the Ethernet shield will automatically obtain an IP address.
To make sure the DHCP lease is properly renewed when needed, be sure to call Ethernet.maintain() regularly.
https://www.arduino.cc/reference/en/libraries/ethernet/
*/
#include <Ethernet.h>
#include "./src/Artila-Matrix310.h"

// The MAC (Media access control) address for the device (array of 6 bytes). 
// This is the Ethernet hardware address which is written on a sticker under your Matrix310.
byte mac[] = { 0x98, 0xf4, 0xab, 0x17, 0x24, 0xc4 };

void setup() {
  // Ethernet LAN chip select
  Ethernet.init(LAN_CS); // pin 5
  Serial.begin(115200);

  // start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
  // Matrix-310 tries connecting the internet with DHCP
  unsigned long dhcpTimeout = 2000;
  if (Ethernet.begin(mac, dhcpTimeout) == 0) {
    //Fail to use DHCP
    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      return;
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
      return;
    }
  } 
  else {  //Matrix310 already connect to the internet
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
}

void loop() {
}
