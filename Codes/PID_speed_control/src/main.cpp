#include <Arduino.h>
#include <PIDController.h>

// Encoder connections
const int encoderPinA = 2;  // Channel A
const int encoderPinB = 3;  // Channel B

// Motor pins
#define in1 7
#define in2 8
#define motor_pwm 6

//  Target RPM
float set_point = 100.0;

//  Measured RPM
float measured_RPM = 0.0;

//  Required PWM
float required_pwm = 0.0;

const float PPR = 514.8;  // as the gear ratio of the motor is 1:46.8, each encoder channel produces 11 signals per revolution, so PPR = 11 * 46.8 = 514.8
unsigned long old_time = 0;

volatile long pos = 0;
void readEncoder();

//  Creating PID object
PIDController speed_PID(3.0, 1.5, 0.0);


void setup() {
  old_time = millis();
  Serial.begin(115200);

  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);
  // Only B is read: A triggered the interrupt (RISING), so its state is always HIGH — no information gained from reading it
  attachInterrupt(digitalPinToInterrupt(encoderPinA), readEncoder, RISING);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(motor_pwm, OUTPUT);

}

void loop() {

  //  Make motor rotate clock wise
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  //  Reading motor RPM
  unsigned long current_time = millis();
  if (current_time - old_time >= 100) {
    noInterrupts();
    long count = pos;
    pos = 0;
    interrupts();

    float dt = (current_time - old_time) / 1000.0;
    measured_RPM = (count / PPR) * (60.0 / dt);
    old_time = current_time;

    //  PID control
    required_pwm = speed_PID.compute(set_point, measured_RPM, dt);
    required_pwm = constrain(required_pwm, 0, 255);
    analogWrite(motor_pwm, (int)required_pwm);

    //  Serial printing
    Serial.print(set_point); Serial.print(",");
    Serial.print(measured_RPM); Serial.print(",");
    Serial.println(required_pwm);

  }

}

void readEncoder() {
  if (digitalRead(encoderPinB))
    pos++;
  else
    pos--;

}