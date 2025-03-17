#include <stdio.h>

#include "colors.h"

#define AT_IMPLEMENTATION
#include "AT.h"

int main(void) {
  char command[] = "AT+COMMANDHERE=PARAM1,param2,PaRaM3,_test_param,END,63,@";

  puts(command);
  puts("");

  AT_Command at = at_command_parse(command);

  printf(COLOR_YELLOW "Parsed: %s\n\n" COLOR_RESET, at_command_pretty_string(at));
  printf(COLOR_GREEN "Serialized: %s\n" COLOR_RESET, at_command_serialize(at));

  return 0;
}
