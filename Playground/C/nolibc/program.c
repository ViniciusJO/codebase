//extern int printf(const char *, ...);
//
//int main(void) {
//  printf("abc\n");
//  return 0;
//}

#include "syscall.h"
#include "io.h"

typedef __builtin_va_list va_list;
/* but you’re not allowed to use the name `va_arg`, technically */
#define va_start __builtin_va_start
#define va_end __builtin_va_end
#define va_copy __builtin_va_copy
#define va_arg __builtin_va_arg


#define asm __asm__


typedef unsigned long long int uint64;

void syscall(enum SYSCALL s, ...) {
  /*va_list va;*/
  /*va_start(va, s);*/
  uint64 v = 0;
  do {
    /*va_arg(va, uint64);*/
    
  } while(v);
}

int write(int fd, const char *buf, int length) {
    int ret;

    asm("mov %1, %%rax\n\t"
        "mov %2, %%rdi\n\t"
        "mov %3, %%rsi\n\t"
        "mov %4, %%rdx\n\t"
        "syscall\n\t"
        "mov %%eax, %0"
        : "=r" (ret)
        : "r" ((uint64) SYSCALL_WRITE), // #define SYS_write 1
          "r" ((uint64) fd),
          "r" ((uint64) buf),
          "r" ((uint64) length)
        : "%rax", "%rdi", "%rsi", "%rdx");

    return ret;
}

static int stringlen(const char *str) {
    const char *i = str;
    for (; *i; i++);
    return i - str;
}

int main(void) {
    const char *msg = "Hello, World!\n";
    /*write(STDOUT, msg, stringlen(msg));*/

    return 0;
}

_Noreturn void exit(int code) {
    /* Infinite for-loop since this function can't return */
    for (;;) {
        asm("mov %0, %%rax\n\t"
            "mov %1, %%rdi\n\t"
            "syscall\n\t"
            :
            : "r" ((uint64) SYSCALL_EXIT),
              "r" ((uint64) code)
            : "%rax", "%rdi");
    }
}

void _start(void) {
     exit(main());
}


//extern void* syscall5(
//    void* number,
//    void* arg1,
//    void* arg2,
//    void* arg3,
//    void* arg4,
//    void* arg5
//);
//
////typedef unsigned long int uintptr; /* size_t */
////typedef long int intptr; /* ssize_t */
////
////static intptr write(int fd, void const* data, uintptr nbytes) {
////    return (intptr)
////        syscall5(
////            (void*)1, /* SYS_write */
////            (void*)(intptr)fd,
////            (void*)data,
////            (void*)nbytes,
////            0, /* ignored */
////            0  /* ignored */
////        );
////}
//
//void _start(void) {
//    write(1, "hello\n", 6);
//}
