#include <Arduino.h>

// Encoder connections
const int encoderPinA = 2;  // Channel A
const int encoderPinB = 3;  // Channel B

volatile long pos = 0;
void readEncoder();

void setup() {
  Serial.begin(115200);

  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);
  // Only B is read: A triggered the interrupt (RISING), so its state is always HIGH — no information gained from reading it
  attachInterrupt(digitalPinToInterrupt(encoderPinA), readEncoder, RISING);

}

void loop() {
  //  atomic read
  noInterrupts();
  long current_position = pos;
  interrupts();
  Serial.println(current_position);
}

void readEncoder() {
  if (digitalRead(encoderPinB))
    pos++;
  else
    pos--;

}