#ifndef __BUFFER_CONTEXT_H__
#define __BUFFER_CONTEXT_H__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus


#include "../../types.h"

typedef struct {
  usize count;
  usize capacity;
  void* data;
} Buffer_Allocator_Context;


#ifdef __cplusplus
}
#endif//__cplusplus
#endif//__BUFFER_CONTEXT_H__
