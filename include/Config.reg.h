#ifndef _PI_REG_H_
#define _PI_REG_H_

/*==PI COEFFICIENTS==*/
#define KP_V (1200.f)
#define KI_V (1500.f)
#define KD_V (10.f)

#define KP (0.0009f)
#define KI (0.00005f)
/*==FREQUENCY DISCRETIZATION==*/
#define Ts_s (0.05f)
#define Ts_ms (Ts_s * 1000)

/*==LIMIT CONTROLLING INFLUENCE==*/
#define MAX_U 255
#define NEG_MAX_U -255

#endif