/*
Rotary Switch Truth Table:

number  SW-4 SW-2 SW-1
0       1    1    1
1       1    1    0
2       1    0    1
3       1    0    0
4       0    1    1
5       0    1    0
6       0    0    1
7       0    0    0

*/
#include "src/Artila-Matrix310.h"

int readRotarySwitch(){
    int total=0;
    if (digitalRead(SW_1) == LOW) { total+=1; }
    if (digitalRead(SW_2) == LOW) { total+=2; }
    if (digitalRead(SW_4) == LOW) { total+=4; }
    return total;
}

void setup()
{
    Serial.begin(115200);
    pinMode(SW_1, INPUT); // SW_1 -> pin 34
    pinMode(SW_2, INPUT); // sw_2 -> pin 26
    pinMode(SW_4, INPUT); // sw_4 -> pin 35
    Serial.print("Rotary switch value is: ");
    Serial.println(readRotarySwitch());
}

void loop(){

}

