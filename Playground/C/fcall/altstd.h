#ifndef ALTSTD_H
#define ALTSTD_H

#define NULL (void*)0

typedef unsigned long long int uint64;

typedef unsigned long size_t;

typedef enum STDSTREAMS {
  STDIN,
  STDOUT,
  STDERR
} STDSTREAMS;

typedef enum SYSCALLS {
  SYSCALL_WRITE = 1
} SYSCALLS;

#define va_start __builtin_va_start
#define va_end __builtin_va_end
#define va_copy __builtin_va_copy
#define va_arg __builtin_va_arg
#define va_list __builtin_va_list

char *int2str(int n, unsigned char base);

int _printf(const char *const fmt, ...);

int _write(int fd, const char* buff, unsigned long length);

int _putchar(int c);

void* _malloc(size_t size);

void* _memcpy(void* dest, const void* src, unsigned long sz);

int _puts(const char *buff);

int _strlen(const char *str);
/*    const char *i = str;*/
/*    for (; *i; i++);*/
/*    return i - str;*/
/*}*/

#endif //ALTSTD_H
