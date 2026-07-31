#include "codebase/colors.h"
#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "codebase/nob.h"

#ifdef __clang__
#define defer
#error Defer not implementable in clang
#else
#define __DEFER(COUNTER)                                                       \
  auto void DEFER_##COUNTER##_FUNCTION(void *);                                \
  int DEFER_##COUNTER##_VAR                                                    \
      __attribute__((__cleanup__(DEFER_##COUNTER##_FUNCTION)));                \
  auto void DEFER_##COUNTER##_FUNCTION(void *)
#define __DEFER_(N) __DEFER(N)
#define defer __DEFER_(__COUNTER__)
#endif

#define defer_block_(start, end)                                               \
  for (int _i_ = ((start), 0); !_i_; (_i_ = 1), (end))

#define defer_block(end) defer_block_(NULL, (end))

int main(int argc, char **argv) {
  NOB_GO_REBUILD_URSELF(argc, argv);

  // defer_block(puts("1")){
  //   puts("2");
  // }

  Nob_Cmd cmd = {0};
  defer { nob_cmd_free(cmd); };

  // puts("Building...");
  // cmd_append(&cmd, "gcc", "-g", "-O0", "-finstrument-functions", "-o", "df",
  // "df.c", "-ldl"); if(!cmd_run(&cmd)) return 1;

  // printf(COLOR_RGB("Runnung...", 0xFFAB00FF, 0xFF000000));
  // puts(COLOR_LIGHT_CYAN"Runnung..."COLOR_RESET);
  // cmd_append(&cmd, "./df");
  // if(!cmd_run(&cmd)) return 1;

  // puts("Building...");
  // cmd_append(&cmd, "gcc", "-o", "t", "macro_ideas.c");
  // if(!cmd_run(&cmd)) return 1;

  // Nob_String_Builder sb = {0};
  // defer_block(nob_sb_free(sb)){
  //   nob_read_entire_file("build.c", &sb);
  //   printf("\n\n%.*s\n\n", (int)sb.count, sb.items);
  // }

  // static void n(int a, int b) {
  //   int k = a + b;
  //   (void)k;
  //   return;
  // }

  // cmd_append(&cmd, "./t");
  // puts("Runnung...");
  // if(!cmd_run(&cmd)) return 1;
}
