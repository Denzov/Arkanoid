#ifndef _PI_REG_H_
#define _PI_REG_H_

/*==PI COEFFICIENTS==*/
#define KP (2.f)
#define KI (2.f)

/*==FREQUENCY DISCRETIZATION==*/
#define Ts_s (0.05f)
#define Ts_ms (Ts_s * 1000)

/*==LIMIT CONTROLLING INFLUENCE==*/
#define MAX_U 255
#define NEG_MAX_U -255

#endif