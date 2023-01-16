/*
Use digitalRead() to read the value from a specified digital pin, either HIGH or LOW.

DIO pin define:
DO1->25 pin
DI1->36 pin
DI2->39 pin
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

void initGPIO()
{
    pinMode(DO1, OUTPUT);
    pinMode(DI1, INPUT);
    pinMode(DI2, INPUT);
}

void setup() {
    initGPIO();
    Serial.begin(115200);    
    readDI();
}

void loop() {

}
