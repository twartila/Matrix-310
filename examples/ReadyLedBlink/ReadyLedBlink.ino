#include"./src/Artila-Matrix310.h"
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_READY, OUTPUT); // pin 27
}

void loop() {
  digitalWrite(LED_READY, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);                      // wait for a second
  digitalWrite(LED_READY, LOW);   // turn the LED off by making the voltage LOW
  delay(1000);                      // wait for a second
}
