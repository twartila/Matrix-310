/*
Used for communication between the Matrix310 and a computer or other devices. 
Matrix310 can use SERIAL1 PINS or SERIAL2 PINS to achieve RS232 communication.
*/

#include "src/Artila-Matrix310.h"
#include "src/crc16.h"
u_int8_t writeMsg[] = { 0x02, 0x03, 0x00, 0x44, 0x00, 0x03, 0x00, 0x00 };
u_int8_t readBuf[11];
int readLen = 0;
void RS485Write() {
  if (Serial2.availableForWrite() >= 8) {
    unsigned short crc = do_crc_table(&writeMsg[0], sizeof(writeMsg) - 2);
    Serial.print("CRC: ");
    Serial.println(crc, HEX);
    *(u_int16_t *)(&writeMsg[6]) = crc;
    Serial.print("write: ");
    for (int i = 0; i < sizeof(writeMsg); i++) {
      Serial.print(*(byte *)(writeMsg + i), HEX);
      Serial.print(" ");
    }
    Serial.println("");
    int writeLen = Serial2.write((byte *)writeMsg, sizeof(writeMsg));
    Serial2.flush();
    Serial.print("write length: ");
    Serial.println(writeLen);
  }
}
void RS485Read() {
  unsigned long RS485Timeout = millis();
  while (1) {
    if (Serial2.available()) {
      readLen = Serial2.readBytes(readBuf, sizeof(readBuf));
      Serial2.flush();
      Serial.print("read: ");
      if (readLen > 0) {
        for (int i = 0; i < readLen; i++)
        {
          Serial.print(*(byte *)(writeMsg + i), HEX);
          Serial.print(" ");
        }
        Serial.println("");
        Serial.print("read length: ");
        Serial.println(readLen);
      }
      
      break;
    }
    // set RS485 timeout for 2 seconds
    if (millis() - RS485Timeout > 2000) {
      Serial.println("read nothing!");
      break;
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(100);
  // Setup Serial2 PINS with the specified baud rates that is depends on the device you connect.
  Serial2.begin(9600);
  //Configures the COM1_RTS pin to behave as an output.
  pinMode(COM1_RTS, OUTPUT);
  Serial.println("RS485 already setup.");
}

void loop() {
  //Write a HIGH value to COM1_RTS pin before the Matrix310 uses the Serial2 PINS to send/write data.
  //Write a LOW value to COM1_RTS pin before the Matrix310 uses the Serial2 PINS to receive/read data.
  digitalWrite(COM1_RTS, HIGH);  // write
  delay(1);
  RS485Write();
  digitalWrite(COM1_RTS, LOW);  // read
  delay(1);
  RS485Read();

  Serial.println("do it again~");
  Serial.println("");
  delay(3000);
}
