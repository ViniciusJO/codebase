#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define JSON_FIELDS(apply) \
  apply(const char*, name) \
  apply(int, age) \
  apply(bool, single) \
  apply(double, salary) \

#define STRUCT_DEFINITION(type, name) type name;
typedef struct {
  JSON_FIELDS(STRUCT_DEFINITION)
} Person;
#undef STRUCT_DEFINITION

void print_person(Person *p) {
  printf("Person {\n");
#define PRINT(type, name) \
  if(0 == strcmp(#type, "int")) {\
    printf("  .%s = %d,\n", #name, p->name); \
  } else if(0 == strcmp(#type, "double")) {\
    printf("  .%s = %.2f,\n", #name, p->name); \
  } else if(0 == strcmp(#type, "bool")) {\
    printf("  .%s = %s,\n", #name, p->name ? "true" : "false"); \
  } else if(0 == strcmp(#type, "const char*") || 0 == strcmp(#type, "const char *")) {\
    printf("  .%s = \"%s\",\n", #name, p->name); \
  }
  JSON_FIELDS(PRINT)
#undef PRINT
  printf("}\n");
}


int main(void) {
  Person p = (Person){
    .name = "Vinicius",
    .single = true,
    .age = 26,
    .salary = 1500,
  };

  print_person(&p);

  return 0;
}
