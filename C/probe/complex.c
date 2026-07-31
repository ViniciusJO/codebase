#include <stdio.h>
#include <complex.h>

int main(void) {
  int complex z = 1 + 8*I;
  printf("Real: %d\n", creal(z));
  printf("Imag: %d\n", cimag(z));
  return 0;
}
