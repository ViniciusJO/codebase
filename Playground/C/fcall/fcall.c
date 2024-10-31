#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>

extern void say_hi();

int sum_in_place(int *a, int b) {
  *a += b;
  return *a;
}

char *readUntilChar(const char *src, char until, int *len) {
  if (src == NULL || strlen(src) == 0) return NULL;

  int idx = 0;
  while(src[idx] != until && src[idx++] != '\0');
  char *str = (char*) malloc((idx + 1) * sizeof(char));
  memcpy(str, src, idx);
  str[idx] = '\0';

  *len = idx;
  return str;
}

int altPrintf(const char * format, ...) {
  puts(format);
  return 0;
}

int main(int argc, char **argv) {
  (void)argc;
  (void)argv;

  /*int i = 0;*/
  /*const char str[] = "ABCHASHDL:J\n\0";*/
  int sz;
  const char *nstr = readUntilChar("Test str", ' ', &sz);
  printf("size: %d => %s\n", sz, nstr);

  return 0;
}
