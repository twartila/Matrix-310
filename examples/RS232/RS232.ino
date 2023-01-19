/*
Used for communication between the Matrix-310 and a computer or other devices. 
Matrix-310 can use SERIAL1 PINS or SERIAL2 PINS to achieve RS232 communication.
Our example use RS232 with null modem to simulate the multi device communication.
*/

#include "./src/Artila-Matrix310.h"
int readLen = 0;
String writeMsg = "Message";
String readStr = "";

void setup() {
  Serial.begin(115200);
  //Setup Serial1 PINS with the specified baud rates, sets data, parity, stop bits ,rx pin and tx pin.
  Serial1.begin(9600,SERIAL_8N1, COM2_RX,COM2_TX);
  Serial1.setTimeout(1000);
  Serial.println("RS232 already setup.");
}

void loop() {
  //Write data to Serial1.
  Serial.print("write: ");
  Serial.println(writeMsg);
  int writeLen = Serial1.print(writeMsg);
  Serial1.flush();
  delay(10);
  Serial.print("data send: ");
  Serial.println(writeLen);
  if (Serial1.available() > 0) {
    //Read data from Serial1.
    readStr = Serial1.readString();
    Serial1.flush();
    readLen = readStr.length();
    Serial.printf("data receive: %i\n", readLen);
    Serial.printf("read: %s\n", readStr);
  } 
  else {
    Serial.println("read nothing!");
  }

  Serial.println("");
  Serial.println("do it again~");
  Serial.println("");
  delay(3000);
}
