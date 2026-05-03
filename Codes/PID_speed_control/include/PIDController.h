#ifndef _MYCLASS_H_
#define _MYCLASS_H_

class PIDController
{
    private:
        float kp;
        float ki;
        float kd;
        float integral;
        float prev_error;

    public:
        PIDController(float kp_value, float ki_value, float kd_value);

        float compute(float set_point, float measured, float dt);

        void reset();

        void set_tunings(float kp_value, float ki_value, float kd_value);

};

#endif
