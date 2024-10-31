
/*#include <stdarg.h>*/
/*#include <stdio.h>*/
/*#include <stdlib.h>*/
/*#include <string.h>*/
/*#include <unistd.h>*/

/*#include <string.h>*/

/*extern void say_hi();*/
/**/
/*int sum_in_place(int *a, int b) {*/
/*  *a += b;*/
/*  return *a;*/
/*}*/

/*static int strlen(const char *str) {*/
/*    const char *i = str;*/
/*    for (; *i; i++);*/
/*    return i - str;*/
/*}*/

/*char *readUntilChar(const char *src, char until, int *len) {*/
/*  if (src == NULL || strlen(src) == 0)*/
/*    return NULL;*/
/**/
/*  int idx = 0;*/
/*  while (src[idx] != until && src[idx++] != '\0')*/
/*    ;*/
/*  char *str = (char *)malloc((idx + 1) * sizeof(char));*/
/*  memcpy(str, src, idx);*/
/*  str[idx] = '\0';*/
/**/
/*  *len = idx;*/
/*  return str;*/
/*}*/



// TODO: implement
/*extern void* malloc(unsigned long);*/
/*extern void* memcpy(void* dest, const void* src, unsigned long sz);*/
/*extern int putchar (int c);*/


/*static void* _malloc(unsigned long sz) {*/
/**/
/**/
/**/
/*}*/

/*static void* _memcpy(void* dest, const void* src, unsigned long sz) {*/
/*  for(size_t i = 0; i < sz; i++) {*/
/*    ((char*)dest)[i] = ((char*)src)[i];*/
/*  }*/
/*  return dest;*/
/*}*/

#include "altstd.h"

int _write(int fd, const char* buff, unsigned long length) {
  int ret;

  __asm__("mov %1, %%rax\n\t"
          "mov %2, %%rdi\n\t"
          "mov %3, %%rsi\n\t"
          "mov %4, %%rdx\n\t"
          "syscall\n\t"
          "mov %%eax, %0"
          : "=r" (ret)
          : "r" ((uint64) SYSCALL_WRITE),
            "r" ((uint64) fd),
            "r" ((uint64) buff),
            "r" ((uint64) length)
          : "%rax", "%rdi", "%rsi", "%rdx"
      );

  return ret;
}

int _putchar(int c) {
  return _write(STDOUT, (char*)&c, 1);
}

int _puts(const char *buff) {
  return _write(STDOUT, buff, _strlen(buff));
}

char *int2str(int n, unsigned char base) {
  if(base > 36) {
    _printf("There is no default representation of alphabets greather than 36");
    return NULL;
  }

  const char alphabet[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  size_t count = 0;
  int digits[255] = {0};
  digits[count++] = '\0';

  for (int v = n < 0 ? -n : n; v; v /= base)
    (digits[count++] = alphabet[(v % base)]);

  if (n < 0)
    digits[count++] = '-';

  char *result = _malloc(count+1);
  int j = 0;
  while (digits[--count] != '\0')
    result[j++] = digits[count];
  result[j] = '\0';
  return result;
}

int _printf(const char *const format, ...) {
  const char *cursor = format;
  va_list val;
  va_start(val, format);
  int i = 0;
  /*puts(format);*/
  while (*cursor != '\0') {
    if (*cursor == '%' && *(++cursor) != '%') {
      switch (*cursor) {
      case 's': {
        char *str = va_arg(val, char *);
        while (*str != '\0')
          _putchar(*(str++));

      } break;
      case 'd': {
        _printf("%s", int2str(va_arg(val, int), 10));
      } break;
      case 'h': {
        _printf("%s", int2str(va_arg(val, int), 16));
      } break;
      case 'o': {
        _printf("%s", int2str(va_arg(val, int), 8));
      } break;
      case 'b': {
        _printf("%s", int2str(va_arg(val, int), 2));
      } break;

      case 'c': {
        _putchar((char)va_arg(val, int));
      } break;
      }
      /*putc('>', stdout);*/
      /*putc('-', stdout);*/
      /*putc('<', stdout);*/
    } else
      _putchar(*cursor);
    cursor++;
    i++;
  }
  va_end(val);
  return i;
}

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  /*int i = 0;*/
  /*const char str[] = "ABCHASHDL:J\n\0";*/
  /*int sz;*/
  /*const char *nstr = readUntilChar("Test str", ' ', &sz);*/
  /*altPrintf("%%size: %b => %s\t %c \t %d\t%s\n", sz, nstr, 'h', 12 * 0x8888888,*/
            /*int2str(34, 2));*/
  _printf("teste: %s, 0%o, 0x%h, %d, 0b%b", "STRING DE TESTE", 26,26,26,26);

  return 0;
}





