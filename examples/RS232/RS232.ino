/*
Used for communication between the Matrix-310 and a computer or other devices. 
Matrix-310 can use Serial1 PINS or SERIAL2 PINS to achieve RS232 communication.
Our example use RS232 with null modem to simulate the multi device communication.
source: https://www.arduino.cc/reference/en/language/functions/communication/serial/ 
*/

#include "./include/Artila-Matrix310.h"
int readLen = 0;
String writeMsg = "Message";
String readStr = "";

void setup() {
  Serial.begin(115200);
  //Setup Serial2 PINS with the specified baud rates, sets data, parity, stop bits ,rx pin and tx pin.
  Serial2.setTimeout(1000);
  Serial2.begin(9600, SERIAL_8N1, COM2_RX, COM2_TX);
  Serial.println("RS232 already setup.");
}

void loop() {
  //Write data to Serial2.
  Serial.print("write: ");
  Serial.println(writeMsg);
  int writeLen = Serial2.print(writeMsg);
  Serial2.flush();
  Serial.print("data send: ");
  Serial.println(writeLen);
  if (Serial2.available() > 0) {
    //Read data from Serial2.
    readStr = Serial2.readString();
    Serial2.flush();
    readLen = readStr.length();
    Serial.printf("data receive: %i\n", readLen);
    Serial.print("read: ");
    Serial.println(readStr);
  } 
  else {
    Serial.println("read nothing!");
  }

  Serial.println("");
  Serial.println("do it again~");
  Serial.println("");
  delay(3000);
}
