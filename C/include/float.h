#ifndef __FLOAT_H__
#define __FLOAT_H__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "./utils.h"

#define float_abs(a) ((a) < 0 ? -(a) : (a))

#define fc_abs_eps(a, b, eps) (float_abs((a) - (b)) <= (eps))
#define fc_rel_eps(a, b, eps) (float_abs((a) - (b))/MAX(float_abs(a), float_abs(b)) <= (eps))

#define feql(a,b,eps) ((a) == (b) ? true : fc_rel_eps((a),(b),(eps)))


#ifdef __cplusplus
}
#endif//__cplusplus
#endif//__FLOAT_H__

//#define FLOAT_IMPLEMENTATIONS
#ifdef FLOAT_IMPLEMENTATIONS
#ifndef __FLOAT_IMP__
#define __FLOAT_IMP__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus



#ifdef __cplusplus
}
#endif//__cplusplus
#endif//__FLOAT_IMP__
#undef FLOAT_IMPLEMENTATIONS
#endif//FLOAT_IMPLEMENTATIONS

