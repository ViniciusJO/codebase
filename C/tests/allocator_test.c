#include <stdbool.h>
#include <stdio.h>
#include <codebase/ANSI.h>

#define ALLOCATOR_IMPLEMENTATIONS
#include <codebase/allocator.h>

int main(void) {
  Allocator libc = heap_allocator_init();
  Allocator arena = arena_allocator_init(&libc, 4096);

  char* alloc = allocate(&arena, 6, ALLIGN_1); 

  alloc[0] = 'h';
  alloc[1] = 'e';
  alloc[2] = 'l';
  alloc[3] = 'l';
  alloc[4] = 'o';
  alloc[5] = '\0';

  printf(FG_GREEN "s: %s\n" FG_RESET, alloc);

  deallocate(&arena,alloc,6,ALLIGN_1);

  printf(FG_BLUE "s: %s\n" FG_RESET, alloc);

  allocator_deinit(&arena);

  printf(FG_CYAN "s: %s\n" FG_RESET, alloc);

  allocator_deinit(&arena);

  printf(FG_YELLOW "s: %s\n" FG_RESET, alloc);

  int *i = (int*)allocate(&libc, 4, ALLIGN_1);
  *i = 8;
  printf(FG_BRIGHTGREEN "allocater *i: %d\n" FG_RESET, *i);
  deallocate(&libc,i,4,ALLIGN_1);
  printf(FG_BRIGHTGREEN "deallocated *i: %d\n\n" FG_RESET, *i);

  const size_t cap = 10 * 1024; // 10 kB
  void* buf[cap];
  Allocator bufa = buffer_allocator_init(buf,cap);

  alloc = allocate(&bufa, 6, ALLIGN_1); 

  alloc[0] = 'h';
  alloc[1] = 'e';
  alloc[2] = 'l';
  alloc[3] = 'l';
  alloc[4] = 'o';
  alloc[5] = '\0';

  printf(FG_GREEN "s: %s\n" FG_RESET, alloc);

  deallocate(&bufa,alloc,6,ALLIGN_1);

  printf(FG_BLUE "s: %s\n" FG_RESET, alloc);

  allocator_deinit(&bufa);

  printf(FG_CYAN "s: %s\n" FG_RESET, alloc);

  allocator_deinit(&bufa);

  printf(FG_YELLOW "s: %s\n" FG_RESET, alloc);

  char *s = allocate(&bufa, 6, ALLIGN_1); 
  s[0] = 'w';
  s[1] = 'o';
  s[2] = 'r';
  s[3] = 'l';
  s[4] = 'd';
  s[5] = ' ';

  printf(FG_CYAN "\nalloc: %s\ns: %s\n" FG_RESET, alloc, s);

  alloc = allocate(&bufa, 6, ALLIGN_1); 

  alloc[0] = 'h';
  alloc[1] = 'e';
  alloc[2] = 'l';
  alloc[3] = 'l';
  alloc[4] = 'o';
  alloc[5] = '\0';

  printf(FG_CYAN "\nalloc: %s\ns: %s\n" FG_RESET, alloc, s);

  printf(FG_CYAN "\nAllocator size: %lu\n" FG_RESET, sizeof(Buffer_Allocator_Context));

  return 0;
}
