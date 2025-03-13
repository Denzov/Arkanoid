#include "../include/PIreg.h"

void PIreg::constrain_u(){
    if(_u > MAX_U){
        _constrained_u = MAX_U;
    }
    else if(_u < NEG_MAX_U){
        _constrained_u = NEG_MAX_U;
    }
    else{
        _constrained_u = _u;
    }
}

void PIreg::passSet(float& set){
    _set = set;
    
}

void PIreg::passCur(float& cur){
    _cur = cur;
}

void PIreg::tick(){
    _err = _set - _cur;

    _P = _err * KP;
    _I = _integrator * KI;

    _u = _P + _I;

    constrain_u();

    if(_u == _constrained_u){
        _integrator += _err * Ts_s;
    }
    else{
        _u = _constrained_u;
    }
}