/**
 * Utilities to parse flags from the arguments passed to
 * the program throught the cli.
 *
 * This library is designed arround the stb header only
 * style. To use it include "flags.h" where its intended
 * to be used and define the following macro before the
 * inclusion in the translation file that the implementation
 * is suposed to be:
 *
 * - To use:
 *       `#include "flags.h"`
 *
 * - On the translation file in wich the implementation is
 *   meant to be:
 *       ```
 *         #define FLAGS_IMPLEMENTATIONS"
 *         #include "flags.h"
 *       ```
 * * IMPORTANT: this case of inclusion and implementation
 *              will be called "inclusion of implementation"
 *              throught the file.
 *
 * * IMPORTANT: the implementation can be defined in only
 *              one translation file. Putting it in multiple
 *              files will lead to linking issues. If it is
 *              intended to have a separate implementation
 *              for a translation unit just define the
 *              following macro before the inclusion of
 *              implementation:
 *
 *              `#define FLAGS_ATTRIBUTES static`
 *               
 *               like:
 *
 *
 *              ```
 *                #define FLAGS_ATTRIBUTES static
 *                #define FLAGS_IMPLEMENTATIONS"
 *                #include "flags.h"
 *              ```
 *
 * The default ammount of possible flags is 32, but it can
 * be changed by definning the folllowing macro before the
 * inclusion of implentation substituting 32 with the desired
 * flags capacity:
 * 
 * ```
 *   #define FLAGS_CAP 32
 *   #define FLAGS_IMPLEMENTATIONS"
 *   #include "flags.h"
 * ```
 *
 * The flags are expected to appear on the cli in the form
 * --flag or in the alternative and shorter form -f.
 *
 * There are three types of flags:
 *
 *   - FLAG_BOOL:     does not receive assignment or argument
 *                    and is set to true if present in the
 *                    command line;
 *
 *   - FLAG_STR:      need to receive assignment or argument
 *                    and is parsed to a string (const char*);
 *
 *   - FLAG_UINT64:   needs argument or assignment and is
 *                    parsed to a uint64_t.
 * 
 * The mechanisms to pass values to the flags are:
 *   
 *   - argument:
 *     the flag and argument are separated with a space
 *     ("command --flag value")
 *
 *   - assignment:
 *     the flags are asigned to the flag with no space
 *     separation ("command --flag=value")
 *
 *
 *
 */
#ifndef __FLAGS_H__
#define __FLAGS_H__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef FLAGS_ATTRIBUTES
#define FLAGS_ATTRIBUTES
#endif//FLAGS_ATTRIBUTES

typedef enum {
  FLAG_BOOL,
  FLAG_UINT64,
  FLAG_STR,
} Flag_Type;

typedef struct {
  uintptr_t default_value;
  bool mandatory;
  char *alt_name;
} Flag_Options;

typedef struct {
  char *name;
  char *description;
  Flag_Type type;
  uintptr_t value;
  uintptr_t def;
  bool parsed;
  Flag_Options options;
} Flag;

FLAGS_ATTRIBUTES void *flag_new(Flag_Type type, char *name, char *description, Flag_Options options);
FLAGS_ATTRIBUTES Flag *flag_find(void *ptr);
FLAGS_ATTRIBUTES void flag_parse(int argc, char **argv);
FLAGS_ATTRIBUTES void flag_print_help(FILE *stream);

#define flag_bool(name, description, ...) ((bool*)flag_new(FLAG_BOOL, name, description, (Flag_Options){ .mandatory = false, .alt_name = NULL, .default_value = 0, __VA_ARGS__ }))
#define flag_uint64(name, description, ...) ((uint64_t*)flag_new(FLAG_UINT64, name, description, (Flag_Options){ .mandatory = false, .alt_name = NULL, .default_value = 0, __VA_ARGS__ }))
#define flag_str(name, description, ...) ((char**)flag_new(FLAG_STR, name, description, (Flag_Options){ .mandatory = false, .alt_name = NULL, .default_value = 0, __VA_ARGS__ }))

#define CAST_DEFAULT_VALUE (uintptr_t)(void*)

// #define flag(type, name, def, description) flag_##type(name,def,description)

#ifdef __cplusplus
}
#endif//__cplusplus
#endif//__FLAGS_H__

#define FLAGS_IMPLEMENTATIONS
#ifdef FLAGS_IMPLEMENTATIONS
#ifndef __FLAGS_IMP__
#define __FLAGS_IMP__
#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <string.h>
#include <inttypes.h>
#include <stdlib.h>
#include <sys/cdefs.h>
#include <limits.h>
#include <errno.h>
#include <assert.h>
#include "colors.h"

// TODO: remove DEBUG
#define DEBUG
#ifdef DEBUG
#define debug_print fprintf
#else
#define debug_print(...)
#endif

#ifndef FLAGS_CAP
#define FLAGS_CAP 32
#endif

#define foreach_flag(iterator_name, loop) do { \
  for(size_t i = 0; i < flags_count; i++) { \
    Flag * const iterator_name = flags + i; \
    { loop } \
  } \
} while(0)

static Flag flags[FLAGS_CAP] = {};
static size_t flags_count = 0;

static char *flag_shift_args(int *argc, char ***argv) {
  assert(*argc > 0);
  char *result = **argv;
  *argv += 1;
  *argc -= 1;
  return result;
}

static Flag *flag_alloc() {
  assert(flags_count < FLAGS_CAP && "Too much flags");
  return flags + flags_count++;
}

FLAGS_ATTRIBUTES void *flag_new(Flag_Type type, char *name, char *description, Flag_Options options) {
  Flag *flag = flag_alloc();
  flag->type = type;
  flag->name = name;
  flag->value = options.default_value;
  flag->description = description;
  flag->parsed = false;
  flag->options = options;
  return &flag->value;
}

FLAGS_ATTRIBUTES Flag *flag_find(void *ptr) {
  foreach_flag(f, { if(&f->value == ptr) return f; });
  return NULL;
}

static size_t local_strlen(const char *s) {
  return strlen(s);
}

FLAGS_ATTRIBUTES void flag_parse(int argc, char **argv) {
  char *program = flag_shift_args(&argc, &argv);

  while(argc > 0) {
    char *f = flag_shift_args(&argc, &argv);
    
    if(*f != '-') {
      fprintf(stderr, "ERROR: %s: invalid argument\n", f);
      exit(1);
    }
    
    debug_print(stdout, COLOR_LIGHT_RED "[%s]\t" COLOR_RESET, f);

    f += 1;

    bool is_short = *f != '-';

    if(!is_short) f += 1;

    bool has_equal = false;
    bool has_post_equal = false;
    uint16_t equal_offset = 0;
    size_t f_size = strlen(f);
    {
      for(size_t j = 0; j < f_size; j++)
        if('=' == f[j]) {
          has_equal = true;
          if(j < f_size - 1) {
            has_post_equal = true;
            equal_offset = j;
          }
          break;
        }
    }

    uint16_t flag_len = equal_offset ? equal_offset : f_size;

    // debug_print(stdout, COLOR_LIGHT_PURPLE "[%s, %d] is_short: %s, expected_len: %lu\n" COLOR_RESET,
    //   f,
    //   equal_offset,
    //   is_short ? "true" : "false",
    //   (unsigned long)expected_len
    // );

    
    debug_print(stdout, COLOR_PURPLE "%s: " COLOR_RESET "(is_short: %s, expected_len: %lu, has_equal: %s) " ,
      f,
      is_short ? "true" : "false",
      (unsigned long)flag_len,
      has_equal ? "true" : "false"
    );

    // foreach_flag(flag_el,
    for(size_t i = 0; i < flags_count; i++){
      Flag *flag_el = flags + i;

      bool has_alt = flag_el->options.alt_name != NULL;
      uint16_t f_len = !is_short ? strlen(flag_el->name) : has_alt ? strlen(flag_el->options.alt_name) : 0;
      if(flag_len != f_len) continue;

      bool match = (
        // full names match
        !is_short &&
        !strcmp(f, flag_el->name)
      ) || (
        // shorts names match
        is_short &&
        flag_el->options.alt_name != NULL &&
        !strcmp(flag_el->options.alt_name, f)
      );

      if(match) {
        debug_print(stdout, "%s = " , flag_el->name);

        uintptr_t value = 0;

        switch(flag_el->type) {
          case FLAG_BOOL: {
            if(has_equal) {
              fprintf(stderr, "ERROR: flag \"%s\" does not receive arguments\n", f);
              exit(1);
            }
            *(bool*)&value = true;
            flag_el->parsed = true;

            debug_print(stdout, "%s", value ? "true" : "false");
          } break;
          case FLAG_STR: {
            char *fn = NULL;
            if(has_equal && has_post_equal) fn = f + equal_offset + 2;
            else {
              if(argc == 0) {
                fprintf(stderr, "ERROR: \"%s\": no argument provided\n", f);
                exit(1);
              }
              fn = flag_shift_args(&argc, &argv);
            }
            *(char**)&value = fn;
            flag_el->parsed = true;

            // debug_print(stdout, "%s", (const char*)value);
          } break;
          case FLAG_UINT64: {
            static_assert(sizeof(unsigned long long) == sizeof(uint64_t), "Only supports x86_64 with unsigned long long of 64bits");

            const char *fn = NULL;
            if(has_equal/* && has_post_equal*/) fn = f + equal_offset + 1;
            else {
              if(argc == 0) {
                fprintf(stderr, "ERROR: \"%s\": no argument provided\n", f);
                exit(1);
              }
              fn = flag_shift_args(&argc, &argv);
            }

            // debug_print(stdout, COLOR_PURPLE "F = %s, FN = %s\n" , f, fn);

            assert(fn != NULL && "ERROR: no argument provided");
            char *endptr;
            unsigned long long result = strtoull(fn, &endptr, 10);
            if(fn == endptr || *endptr != '\0') {
              fprintf(stderr, "ERROR: \"%s\": not a valid number\n", fn);
              exit(1);
            }
            if(result == ULLONG_MAX && errno == ERANGE) {
              fprintf(stderr, "ERROR: \"%s\": 64 bit unsigned integer overflow\n", fn);
              exit(1);
            }

            flag_el->value = result;
            flag_el->parsed = true;

            // debug_print(stdout, "%llu (%llu)", (unsigned long long)value, result);
          } break;
          default: assert(false && "unrechable");
        }

        debug_print(stdout, "\n\n");

        if(flag_el->parsed) {
          flag_el->def = flag_el->value;
          flag_el->value = value;
          break;
        }
      }
    }
    //);


  }

  foreach_flag(flag_el, {
    if(flag_el->type != FLAG_BOOL && flag_el->options.mandatory && !flag_el->parsed) {
      fprintf(stderr, "Flag -%s is needed\n", flag_el->name);
      exit(1);
    }
  });

}

FLAGS_ATTRIBUTES void flag_print_help(FILE *stream) {
  fprintf(stream, "Usage help:\n\n");

  int max_name_len = 0;
  int max_altname_len = 0;
  foreach_flag(f, {
      uint8_t l = strlen(f->name);
      if(l > max_name_len) max_name_len = l;
      if(f->options.alt_name) {
        uint8_t al = strlen(f->options.alt_name);
        if(al > max_altname_len) max_altname_len = al;
      }
  });


  const char *padding_str = "                                                                ";
  foreach_flag(flag_el, {
    fprintf(stream, "\t--%s%.*s",
      flag_el->name,
      (int)(max_name_len + 2 - strlen(flag_el->name)), padding_str
    );
    fprintf(stream, "%c%s",
      flag_el->options.alt_name ? '-' : ' ',
      flag_el->options.alt_name ? flag_el->options.alt_name : ""
    );
    fprintf(stream, "%.*s\t", (int)(max_altname_len + 2 - (flag_el->options.alt_name ? strlen(flag_el->options.alt_name) : 0)), padding_str);

    switch(flag_el->type) {
      case FLAG_BOOL: {
        fprintf(stream, "[bool]\n\t\t\t%s\n\t\t\tDefault: %s\n", flag_el->description, (flag_el->parsed ? flag_el->def : flag_el->value) ? "true" : "false");
      } break;
      case FLAG_UINT64: {
        fprintf(stream, "[uint64_t]\n\t\t\t%s\n\t\t\tDefault: %" PRIu64 "\n", flag_el->description, (uint64_t)(flag_el->parsed ? flag_el->def : flag_el->value));
      } break;
      case FLAG_STR: {
        char *str = (char*)(flag_el->parsed ? flag_el->def : flag_el->value);
        fprintf(stream, "[string]\n\t\t\t%s\n\t\t\tDefault: \"%s\"\n", flag_el->description, str ? str : "");
      } break;
    }

    fprintf(stream, "\n");
  });
  fprintf(stream, "\n");
}

#ifdef __cplusplus
}
#endif//__cplusplus
#endif//__FLAGS_IMP__
#undef FLAGS_IMPLEMENTATIONS
#endif//FLAGS_IMPLEMENTATIONS

