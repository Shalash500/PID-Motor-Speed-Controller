#include "PIDController.h"

PIDController::PIDController(float kp_value, float ki_value, float kd_value)
{
    kp = kp_value;
    ki = ki_value;
    kd = kd_value;
    integral = 0;
    prev_error = 0;
}

float PIDController::compute(float set_point, float measured, float dt)
{
    float error = set_point - measured;
    integral += error * dt;
    if(integral > 200.0) integral = 200.0;
    if(integral < -200.0) integral = -200.0;
    float derivative = (error - prev_error) / dt;
    float output = kp * error + ki * integral + kd * derivative;
    prev_error = error;
    return output;
}

void PIDController::reset()
{
    integral = 0;
    prev_error = 0;
}

void PIDController::set_tunings(float kp_value, float ki_value, float kd_value)
{
    kp = kp_value;
    ki = ki_value;
    kd = kd_value;
}
