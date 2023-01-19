/*
Use digitalRead() to read the value from a specified digital pin, either HIGH or LOW.

DIO pin define:
DO1->pin 25
DI1->pin 35
DI2->pin 39
*/
#include "src/Artila-Matrix310.h"

int di1_status = 0;
int di2_status = 0;

void readDI(){
    di1_status = digitalRead(DI1);
    di2_status = digitalRead(DI2);
    
    Serial.println((String)"DI1: " + di1_status);
    Serial.println((String)"DI2: " + di2_status);
}

void setup() {
    pinMode(DO1, OUTPUT);
    pinMode(DI1, INPUT);
    pinMode(DI2, INPUT);
    Serial.begin(115200);    
    readDI();
}

void loop() {

}
