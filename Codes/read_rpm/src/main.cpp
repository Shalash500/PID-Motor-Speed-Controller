#include <Arduino.h>

// Encoder connections
const int encoderPinA = 2;  // Channel A
const int encoderPinB = 3;  // Channel B

const float PPR = 514.8;  // as the gear ratio of the motor is 1:46.8, each encoder channel produces 11 signals per revolution, so PPR = 11 * 46.8 = 514.8
unsigned long old_time = 0;

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
  unsigned long current_time = millis();

  if (current_time - old_time >= 1000) {
    noInterrupts();
    long count = pos;
    pos = 0;
    interrupts();

    float RPM = (count * 60000.0) / (PPR * (current_time - old_time));
    old_time = current_time;
    Serial.println(RPM);
  }
}

void readEncoder() {
  if (digitalRead(encoderPinB))
    pos++;
  else
    pos--;

}