

#include "src/Artila-Matrix310.h"

int readSwitch(){
    int total=0;
    if (digitalRead(SW_1) == LOW) { total+=1; }
    if (digitalRead(SW_2) == LOW) { total+=2; }
    if (digitalRead(SW_4) == LOW) { total+=4; }
    return total;
}

//test rotary switch
void testRotarySW() {
    int bulleye = random(0, 7);
    Serial.printf("Please change rotary switch to %d\n", bulleye);
    
    while ( readSwitch() != bulleye ) ;
    Serial.println("Rotary switch test is OK");
}
void setup()
{
    Serial.begin(115200);
    pinMode(SW_1, INPUT); // SW_1
    pinMode(SW_2, INPUT); // sw_2
    pinMode(SW_4, INPUT); // sw_4
}

void loop()
{
  testRotarySW();
}

