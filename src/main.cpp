#include <Arduino.h>

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600);
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial1.println("Tu n'as pas grillé la Teensy 4.0, enfin pas la deuxième.");

}
