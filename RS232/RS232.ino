/*
Used for communication between the Matrix310 and a computer or other devices. 
Matrix310 can use SERIAL1 PINS or SERIAL2 PINS to achieve RS232 communication.
Our example use RS232 with null modem to simulate the multi device communication.
*/

#include "./src/Artila-Matrix310.h"
char msg[] = "Message\0";
char buf[64];
int readLen = 0;
void setup() {
  Serial.begin(115200);
  Serial.setTimeout(100);
  //Setup Serial1 PINS with the specified baud rates, sets data, parity, stop bits ,rx pin and tx pin.
  Serial1.begin(9600,SERIAL_8N1, COM2_RX,COM2_TX);
  Serial1.setTimeout(100);
  Serial.println("RS232 already setup.");
}

void loop() {
  //Write data to Serial1.
  int writeLen = Serial1.write(msg);
  Serial1.flush();
  delay(10);
  Serial.print("data send: ");
  Serial.println(writeLen);
  if (Serial1.available() > 0) {
    //Read data from Serial1 and save to buffer.
    readLen = Serial1.readBytes(buf, sizeof(buf));
    Serial1.flush();
    Serial.print("data receive: ");
    Serial.println(readLen);
  } 
  else {
    Serial.println("read nothing!");
  }
  
  if (readLen > 0) {
    //Print the data from buffer.
    Serial.print(buf);
    Serial.print(" ");
  }

  Serial.println("");
  Serial.println("do it again~");
  Serial.println("");
  delay(3000);
}
