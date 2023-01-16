/*
Used for communication between the Matrix310 and a computer or other devices. 
Matrix310 uses SERIAL2 PINS to achieve RS485 communication.
*/

#include "./src/Artila-Matrix310.h"
int readLen = 0;
String writeMsg = "Message";
String readStr = "";

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(100);
  //Setup Serial2 PINS with the specified baud rates that is depends on the device you connect.
  Serial2.begin(115200);
  Serial2.setTimeout(100);
  //Configures the COM1_RTS pin to behave as an output.
  pinMode(COM1_RTS, OUTPUT);
  //Write a HIGH value to COM1_RTS pin before the Matrix310 uses the Serial2 PINS to send/write data.
  //Write a LOW value to COM1_RTS pin before the Matrix310 uses the Serial2 PINS to receive/read data.
  digitalWrite(COM1_RTS, HIGH);
  delay(0.01);
  Serial.println("RS485 already setup.");
}

void loop() {
  int writeLen = Serial2.print(writeMsg);
  Serial2.flush();
  delay(32);
  Serial.print("data send: ");
  Serial.println(writeLen);

  digitalWrite(COM1_RTS, LOW);  // read
  delay(0.01);

  unsigned long Time485;
  Time485 = millis();

  while (1) {
    if (Serial2.available()) {
      readStr = Serial2.readString();
      Serial2.flush();
      delay(32);
      readLen = readStr.length()-2;
      Serial.printf("data receive: %i\n", readLen);
      Serial.printf("read: %s\n", readStr);
      
      break;
    }
    if (millis() - Time485 > 5000) {
      Serial.println("read nothing!");
      break;
    }
  }
  digitalWrite(COM1_RTS, HIGH);  // write
  delay(0.01);
  Serial.println("");
  Serial.println("do it again~");
  Serial.println("");
  delay(3000);
}


