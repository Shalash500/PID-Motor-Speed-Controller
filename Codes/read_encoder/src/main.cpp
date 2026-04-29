#include <Arduino.h>

// Encoder connections
const int encoderPinA = 2;  // Channel A
const int encoderPinB = 3;  // Channel B

volatile long encoderCount = 0;
void readEncoder();

void setup() {
  Serial.begin(9600);

  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);

}

void loop() {

  readEncoder();

}

void readEncoder() {
  int a = digitalRead(encoderPinA);
  int b = digitalRead(encoderPinB);
  Serial.print(a*5);
  Serial.print(" ");
  Serial.print(b*5);
  Serial.println();
}