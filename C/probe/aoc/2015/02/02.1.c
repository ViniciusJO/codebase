#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRING_SIZE_LIMIT 256

char *tok_in_bounds(char *start, char *end) {
  short sz = end - start;
  if(sz < 0) {
    puts("ERROR: The end pointer is behind the start pointer\n");
    return NULL;
  }
  if(sz > STRING_SIZE_LIMIT) {
    printf("%d bytes is to long of an interval to take the token\n", sz);
    return NULL;
  }
  char *tok = (char*)malloc(sz+1);
  memcpy(tok, start, sz+1);
  tok[sz] = '\0';
  return tok;
}

int string_2_int(const char * const s) {
  int value = 0;

  for(const char *digit = s; '\0' != *digit; digit++) {
    if(*digit < '0' || *digit > '9') continue;
    
    value *= 10;
    value += *digit - '0';
  }

  return value;
}

#define MIN(A,B) ((A) < (B) ? (A) : (B))

int main() {

  FILE *f = fopen("input.txt", "r");
  if(NULL == f) printf("Not able to open the file.");

  int area = 0;
  char str[16] = {0};

  char *start, *end;
  int w, h, l;
  int a0, a1, a2;
  while(fgets(str,16,f)) {

    start = str;
    end = strchr(start, 'x');
    char *s_w = tok_in_bounds(start, end);
    w = string_2_int(s_w);
    free(s_w);
    
    start = end + 1;
    end = strchr(start, 'x');
    char *s_l = tok_in_bounds(start, end);
    l = string_2_int(s_l);
    free(s_l);
    
    start = end + 1;
    end = strchr(start, 'x');
    char *s_h = tok_in_bounds(start, end);
    h = string_2_int(s_h);
    free(s_h);
    
    // 2*l*w + 2*w*h + 2*h*l
    a0 = l*w;
    a1 = w*h;
    a2 = h*l;

    area += 2*a0 + 2*a1 + 2*a2 + MIN(MIN(a0,a1),a2);
  }

  printf("The elves need %d square foot of wraping paper\n", area);

  return 0;
}
