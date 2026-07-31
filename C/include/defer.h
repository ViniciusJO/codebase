#ifndef __DEFER_H__
#define __DEFER_H__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#define defer_block(expression) \
  for(bool i_##__LINE__ = true; i_##__LINE__; (i_##__LINE__ = false), expression)

#ifdef __cplusplus
}
#endif//__cplusplus
#endif//__DEFER_H__
