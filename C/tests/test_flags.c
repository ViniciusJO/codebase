#define FLAGS_ATTRIBUTES static
#define FLAGS_IMPLEMENTATION
#include "codebase/flags.h"

#include <stdlib.h>

void usage(FILE * const stream) {
  fprintf(stream, "Usage: ft [OPTIONS]\n\nOPTIONS:\n");
  flag_print_help(stream);
}

int main(int argc, char **argv) {
  bool *help = flag_bool("help", "print this help message", .alt_name = "h");
  bool *b = flag_bool("B", "description of B", .alt_name = "b", .mandatory = true);
  bool *b_i = flag_bool("B_I", "description of B_I", .alt_name = "bi");
  *b_i = true;

  uint64_t *ui = flag_uint64("ui", "description of UI");
  uint64_t *ui_i = flag_uint64("ui_i", "description of UI_I", .alt_name="u");
  *ui_i = 8;

  char **str = flag_str("str", "description of STR", .alt_name = "alt");
  char **str_i = flag_str("str_i", "description of STR_I", .alt_name = "s");
  *str_i = (char*)malloc(256);
  sprintf(*str_i, "Teste");
  char **str_i2 = flag_str(
    "str_i2",
    "description of STR_I2",
    .default_value = CAST_DEFAULT_VALUE"string padrão",
    .alt_name = "S"
  );

  flag_str("str", "description of STR", .alt_name = "alt");

  // flag_print_help(stdout);
  //

  // int VAR = 8;
  // printf(COLOR_LIGHT_GREEN "Found B flag: %p\n" COLOR_RESET, flag_find(b));
  // printf(COLOR_LIGHT_RED "Found VAR flag: %p\n\n" COLOR_RESET, flag_find(&VAR));

  flag_parse(argc, argv);

  if(*help) {
    usage(stdout);
    exit(0);
  }

  fprintf(stdout, "STR state: %s\n", *str);
  fprintf(stdout, "UI_I state: %" PRIu64 "\n", *ui_i);

  return 0;
}
