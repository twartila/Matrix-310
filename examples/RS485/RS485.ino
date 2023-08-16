/*
Used for communication between the Matrix310 and a computer or other devices.
Matrix310 can use SERIAL1 PINS or Serial2 PINS to achieve RS485 communication.
source: https://www.arduino.cc/reference/en/language/functions/communication/serial/
*/

#include "./include/Artila-Matrix310.h"
#include "./include/crc16.h"
// You may change the writeMsg according to your device setting.
u_int8_t writeMsg[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00};

void RS485Write()
{
  if (Serial1.availableForWrite() >= 8)
  {
    unsigned short crc = do_crc_table(&writeMsg[0], sizeof(writeMsg) - 2);
    Serial.printf("CRC: %04X\n", crc);
    *(u_int16_t *)(&writeMsg[6]) = crc;
    Serial.print("write: ");
    for (int i = 0; i < sizeof(writeMsg); i++)
    {
      Serial.printf("%02X ", *(byte *)(writeMsg + i));
    }
    Serial.println("");
    int writeLen = Serial1.write((byte *)writeMsg, sizeof(writeMsg));
    Serial1.flush();
    Serial.print("write length: ");
    Serial.println(writeLen);
  }
}
void RS485Read()
{
  unsigned int readBufLen = writeMsg[5] * 2 + 5;
  u_int8_t *readBuf = (u_int8_t *)malloc(readBufLen);
  int readLen = 0;
  if (Serial1.available())
  {
    readLen = Serial1.readBytes(readBuf, readBufLen);
    Serial1.flush();
    Serial.print("read: ");
    if (readLen > 0)
    {
      for (int i = 0; i < readLen; i++)
      {
        Serial.printf("%02X ", *(byte *)(readBuf + i));
      }
      Serial.println("");
      Serial.print("read length: ");
      Serial.println(readLen);
    }
    else
    {
      Serial.println("read nothing!");
    }
  }
  free(readBuf);
}
void setup()
{
  Serial.begin(115200);
  // Setup Serial1 PINS with the specified baud rates that is depends on the device you connect.
  Serial1.setTimeout(1000);
  Serial1.begin(9600, SERIAL_8N1, COM1_RX, COM1_TX);
  // Configures the COM1_RTS pin to behave as an output.
  pinMode(COM1_RTS, OUTPUT);
  Serial.println("RS485 already setup.");
}
void loop()
{
  // Write a HIGH value to COM1_RTS pin before the Matrix310 uses the Serial1 PINS to send/write data.
  // Write a LOW value to COM1_RTS pin before the Matrix310 uses the Serial1 PINS to receive/read data.
  digitalWrite(COM1_RTS, HIGH); // write
  delay(1);
  RS485Write();
  digitalWrite(COM1_RTS, LOW); // read
  delay(1);
  RS485Read();
  Serial.println("do it again~");
  Serial.println("");
  delay(3000);
}
