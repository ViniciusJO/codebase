#define FOR(var, lim, body)                                                    \
  for(int var = 0; var < lim; var++) {                                        \
    body                                                                       \
  }

#define FOR_RANGE(var, inf, sup, body)                                         \
  for(int var = inf; var <= sup; var++) {                                     \
    body                                                                       \
  }

extern int printf(const char *, ...);

int main(void) {

  FOR(k, 10, { printf("%d", k); });

  printf("\n\n");
  FOR(p, 26, { printf("%c", 'a' + p); });
  FOR(p, 26, { printf("%c", 'A' + p); });
  FOR(p, 10, { printf("%c", '0' + p); });

  printf("\n\n");
  FOR_RANGE(a2z, 'a', 'z', { printf("%c", a2z); });
  return 0;
}
