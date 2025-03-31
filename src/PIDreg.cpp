#include "../include/PIDreg.h"

void PIDreg::constrain_u(){
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

void PIDreg::passSet(float set){
    _set = set;
}

void PIDreg::passCur(float cur){
    _cur = cur;
}

float PIDreg::getU(){
    return _u;
}

PIDreg* PIDreg::tick(){
    _err = _set - _cur;

    _P = _err * KP;
    _I = _integrator * KI;
    _D = (_err - _prev_err) / Ts_s;

    _u = _P + _I + _D;

    constrain_u();

    if(_u == _constrained_u){
        _integrator += _err * Ts_s;
    }
    else{
        _u = _constrained_u;
    }

    _prev_err = _err;

    return this;
}