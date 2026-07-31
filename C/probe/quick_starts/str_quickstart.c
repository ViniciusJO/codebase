/*
 * str.h - String Type Definitions Quickstart
  *
 * str.h provides two string type definitions for C programming:
 * - str: A dynamic, mutable string (similar to ArrayList)
 * - str_view: A non-owning view of a string (like a substring reference)
 *
 * These types are designed to work with al.h (array list) for memory
 * management and provide a modern way to handle strings in C without
 * manual pointer management.
 *
 * ## Quick Start
 *
  * ### Using Dynamic Strings
 *
 *   #include \"str.h\"
 *   #include <stdio.h>
 *   #include <string.h>
 *
 *   int main(void) {
 *       // Create dynamic string
 *       str mystr = {0};
 *
 *       // Build string (using al_ macros since str uses al structure)
 *       const char *text = \"Hello\";
 *       for (size_t i = 0; i < strlen(text); i++) {
 *           al_append(&mystr, text[i]);
 *       }
 *       al_append(&mystr, '\\0');  // Null terminate
 *
 *       // Use as C string
 *       printf(\"%s\
\", mystr.items);
 *       printf(\"Length: %zu\
\", mystr.count - 1);  // -1 for null terminator
 *
 *       // Cleanup
 *       al_free(&mystr);
 *       return 0;
 *   }
 *
 * ## Data Types
 *
 * ### str - Dynamic String
 *
 *   typedef struct {
 *       char *items;     // String buffer
 *       size_t count;    // Number of characters (including null if present)
 *       size_t capacity; // Allocated size
 *   } str;
 *
 * Represents a mutable, dynamically-allocated string. Works with al_
 * array list macros for manipulation.
 *
 * ### str_view - String View
 *
 *   typedef struct {
 *       char *items;     // Pointer to string (does not own)
 *       size_t count;    // Length of substring
 *   } str_view;
 *
 * A non-owning view of a string or substring. Useful for passing string
 * references without allocation or copying.
 *
 * ## Common Patterns
 *
 * ### Building a Dynamic String
 *
  *   #include \"str.h\"
 *   #include <stdio.h>
 *   #include <string.h>
 *
 *   // Assuming al.h is included for the array list macros
 *   al_define(char, str);  // Actually str is predefined
 *
 *   str build_greeting(const char *name) {
 *       str result = {0};
 *
 *       const char *greeting = \"Hello, \";
 *       for (size_t i = 0; greeting[i]; i++) {
 *           al_append(&result, greeting[i]);
 *       }
 *
 *       for (size_t i = 0; name[i]; i++) {
 *           al_append(&result, name[i]);
 *       }
 *
 *       al_append(&result, '!');
 *       al_append(&result, '\\0');
 *
 *       return result;
 *   }
 *
 *   int main(void) {
 *       str greeting = build_greeting(\"Alice\");
 *       printf(\"%s\
\", greeting.items);  // \"Hello, Alice!\"
 *       al_free(&greeting);
 *       return 0;
 *   }
 *
 * ### String View for Substrings
 *
  *   // Create a view of part of a string
 *   const char *text = \"Hello, World!\";
 *   str_view hello = {.items = (char*)text, .count = 5};
 *
 *   // Use without copying or allocating
 *   for (size_t i = 0; i < hello.count; i++) {
 *       printf(\"%c\", hello.items[i]);
 *   }
 *   printf(\"\
\");  // Prints: \"Hello\"
 *
 * ### Converting Between Types
 *
  *   // From str to str_view
 *   str dynamic = {0};
 *   // ... build string ...
 *   str_view view = {.items = dynamic.items, .count = dynamic.count - 1};
 *
 *   // From C string to str_view
 *   const char *cstr = \"example\";
 *   str_view view = {.items = (char*)cstr, .count = strlen(cstr)};
 *
 * ### String Concatenation
 *
 *   str concat_strings(const str *s1, const str *s2) {
 *       str result = {0};
 *
 *       // Copy first string (excluding null terminator if present)
 *       size_t count1 = s1->count;
 *       if (s1->count > 0 && s1->items[s1->count - 1] == '\\0') {
 *           count1--;  // Skip null terminator
 *       }
 *       al_append_many(&result, s1->items, count1);
 *
 *       // Copy second string
 *       size_t count2 = s2->count;
 *       if (s2->count > 0 && s2->items[s2->count - 1] == '\\0') {
 *           count2--;
 *       }
 *       al_append_many(&result, s2->items, count2);
 *
 *       al_append(&result, '\\0');
 *       return result;
 *   }
 *
 * ### String Searching
 *
 *   size_t find_char(const str *s, char c) {
 *       for (size_t i = 0; i < s->count; i++) {
 *           if (s->items[i] == c) return i;
 *       }
 *       return (size_t)-1;  // Not found
 *   }
 *
 * ### String Transformation
 *
  *   void str_to_upper(str *s) {
 *       for (size_t i = 0; i < s->count; i++) {
 *           if (s->items[i] >= 'a' && s->items[i] <= 'z') {
 *               s->items[i] = s->items[i] - 'a' + 'A';
 *           }
 *       }
 *   }
 *
  * ## Important Notes
 *
 * - str uses the same structure as al.h array lists
 * - Memory is managed with al_append(), al_free(), etc.
 * - null termination is optional; you must add it explicitly
 * - str_view does not own memory; don't free the underlying pointer
 * - Both types work seamlessly with al_* macros
 * - No string-specific functions provided; use standard C string functions
 * - count includes null terminator if you add it
 *
 * ## Limitations
 *
 * - No built-in string functions (strlen, strcpy, etc.)
 * - No automatic null termination
 * - No string comparison helpers
 * - No substring extraction functions
 * - Manual memory management required
 * - No encoding/decoding support
 *
 * ## When to Use
 *
 * Use str when:
 * - You need dynamic, growable strings
 * - You prefer modern C container types
 * - You want to avoid repeated allocation calls
 * - You're already using al.h in your project
 *
 * Use str_view when:
 * - You need to reference a substring without copying
 * - You're parsing or tokenizing strings
 * - You want lightweight string references
 * - You're building views from larger buffers
 *
 * Use traditional C strings (char*) when:
 * - You work with static or extern strings
 * - You interact with C standard library functions
 * - Memory allocation is not needed
 * - Performance is critical and allocations must be minimized
 *
 * ## Related Functions
 *
 * String-related operations are provided via:
 * - al.h macros (append, insert, remove, etc.)
 * - Standard C library (strlen, strcpy, strcmp, etc.)
 * - Custom functions you write for your domain
 *
 */
