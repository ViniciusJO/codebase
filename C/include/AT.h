#ifndef __AT_H__
#define __AT_H__

#include <stddef.h>

typedef struct {
  char *command;
  char **param;
  size_t param_count;
} AT_Command;

char *tok_in_bounds(char *start, char *end);

AT_Command at_command_parse(char *str);
char *at_command_serialize(AT_Command at);
char *at_command_pretty_string(AT_Command at);

#ifdef AT_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRING_SIZE_LIMIT 256

char *tok_in_bouds(char *start, char *end) {
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

AT_Command at_command_parse(char *str) {

  char *param_sep = strchr(str, '=');

  str += 3;

  AT_Command at = {.command = strtok(str, param_sep ? "=" : ""),
                   .param_count = 0};

  if (param_sep) {
    str = str + strlen(str) + 1;
    char *current_param_sep = param_sep;
    char *next_param_sep = strchr(str, ',');
    for(;next_param_sep != NULL;) {
      next_param_sep = strchr(current_param_sep+1, ',');
      if(next_param_sep==NULL) break;
      at.param = (char**)realloc(at.param, ++at.param_count * sizeof(char*));
      at.param[at.param_count - 1] = tok_in_bouds(current_param_sep+1, next_param_sep);
      current_param_sep = next_param_sep;
    }
    at.param = (char**)realloc(at.param, ++at.param_count*sizeof(char*));
    at.param[at.param_count - 1] = tok_in_bouds(current_param_sep+1, current_param_sep+strlen(current_param_sep));
  }

  return at;
}

char *at_command_serialize(const AT_Command at) {
  char *str = (char *)malloc(512);
  size_t len = 0;
  len += sprintf(str, "AT+%s%s",at.command,at.param_count > 0 ? "=" : "");
  short param_count = at.param_count;
  while(--param_count >= 0)
    len += sprintf(str+len,"%s%s", at.param[at.param_count-param_count-1], param_count != 0 ? "," : "");
  char *ret = strdup(str);
  free(str);
  return ret;
}

char *at_command_pretty_string(AT_Command at) {
  char *str = (char *)malloc(512);
  size_t len = 0;
  len += sprintf(str, "{\n    command: %s\n    param_count: %lu\n", at.command, at.param_count);
  if (at.param_count) {
    len += sprintf(str + len, "    parameters: {\n");
    for (size_t i = 0; i < at.param_count; i++) {
      len += sprintf(str + len, "        %s%c\n", at.param[i],
                     i < at.param_count - 1 ? ',' : ' ');
    }
    sprintf(str + len, "    }\n}");
  } else {
    sprintf(str + len, "\n    parameters: {}\n}");
  }
  char *ret = strdup(str);
  free(str);
  return ret;
}

#endif //AT_IMPLEMENTATION
#endif //__AT_H__
