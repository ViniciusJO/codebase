extern "C" int printf(const char*, ...);

int add(const int a, const int b) { return a + b; }

void printHello(const char *name) {
  printf("Hello %s!\n", name);
}

int x;
volatile int y;

void k(){  }
void l(){  }
