int printf(const char *, ...);

int main(void) {
  float f = 87.0;
  printf("%d\n", *(int*)&f);
  return 0;
}
