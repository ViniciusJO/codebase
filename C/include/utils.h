#ifndef __UTILS_H__
#define __UTILS_H__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus


#define MAX(A,B) ((A) > (B) ? (A) : (B))
#define MIN(A,B) ((A) < (B) ? (A) : (B))

static inline void __$_unused(void* f, ...) { (void)f; return; }
#define UNUSED(...) __$_unused(NULL,__VA_ARGS__)

#define TODO(MSG)
#define UNIMPLEMENTED(MSG)

#define STRUCT(...) { __VA_ARGS__ }


#ifdef __cplusplus
}
#endif//__cplusplus
#endif//__UTILS_H__

