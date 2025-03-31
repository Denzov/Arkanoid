#ifndef _PID_REGULATOR_H_
#define _PID_REGULATOR_H_

#include <iostream> 
#include "Config.reg.h"

class PIDreg{
private:
    float _integrator = 0;
    
    float _set = 0;
    float _cur = 0;

    float _err = 0;
    float _prev_err = 0;

    float _P = 0;
    float _I = 0;
    float _D = 0;

    float _u = 0;
    float _constrained_u = 0;

    void constrain_u();
public:
    void passSet(float _set);
    void passCur(float _cur);  

    float getU();
    
    PIDreg* tick(); 
};

#endif // !_PID_REGULATOR_H_