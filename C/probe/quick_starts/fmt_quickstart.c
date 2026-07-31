/*
 * fmt.h - Printf Format String Macro Quickstart
 *
 * fmt.h provides the FMT() macro which uses C11 _Generic to automatically
 * select the correct printf format specifier based on the variable type.
 * It eliminates the need to remember format strings for different types
 * and provides compile-time type checking.
 *
 * ## Quick Start
 *
 * ### Basic Usage
 *
 *   #include <stdio.h>
 *   #include \"fmt.h\"
 *
 *   int main(void) {
 *       int i = 42;
 *       double d = 3.14;
 *       char *s = \"hello\";
 *
 *       printf(FMT(i), i);     // Automatically uses \"%d\"
 *       printf(FMT(d), d);     // Automatically uses \"%f\"
 *       printf(FMT(s), s);     // Automatically uses \"%s\"
 *
 *       return 0;
 *   }
 *
 * ### Comparison: Manual vs FMT
 *
 *   // Without FMT: Error-prone, easy to mismatch
 *   int age = 30;
 *   printf(\"%d\
\", age);        // Must remember \"%d\"
 *
 *   long count = 1000000L;
 *   printf(\"%ld\
\", count);      // Must remember \"%ld\" not \"%d\"
 *
 *   // With FMT: Type-safe, no mistakes
 *   int age = 30;
 *   printf(FMT(age), age);
 *
 *   long count = 1000000L;
 *   printf(FMT(count), count);  // Same pattern, different format
 *
 * ## Supported Types
 *
  * ### Boolean Types
 * - _Bool (uses \"%d\")
 *
 * ### Integer Types
 * - char (uses \"%hhd\")
 * - signed char (uses \"%hhd\")
 * - unsigned char (uses \"%hhu\")
 * - short (uses \"%hd\")
 * - unsigned short (uses \"%hu\")
 * - int (uses \"%d\")
 * - unsigned int (uses \"%u\")
 * - long (uses \"%ld\")
 * - unsigned long (uses \"%lu\")
 * - long long (uses \"%lld\")
 * - unsigned long long (uses \"%llu\")
 *
  * ### Floating Point Types
 * - float (uses \"%f\")
 * - double (uses \"%f\")
 * - long double (uses \"%Lf\")
 *
 * ### String Types
 * - char* (uses \"%s\")
 * - const char* (uses \"%s\")
 *
 * ### Pointer Types
 * - Any other pointer (uses \"%p\")
 *
 * ## Common Patterns
 *
 * ### Logging Integers
 *
 *   int error_code = 42;
 *   long timestamp = 1234567890L;
 *
 *   printf(\"Error code: \" FMT(error_code) \"\
\", error_code);
 *   printf(\"Timestamp: \" FMT(timestamp) \"\
\", timestamp);
 *
 * ### Generic Debug Macro
 *
 *   #define DEBUG_VAR(var) do { \\
 *       printf(#var \" = \" FMT(var) \"\
\", var); \\
 *   } while(0)
 *
 *   int x = 42;
 *   double y = 3.14;
 *   char *name = \"test\";
 *
 *   DEBUG_VAR(x);     // Prints: x = 42
 *   DEBUG_VAR(y);     // Prints: y = 3.140000
 *   DEBUG_VAR(name);  // Prints: name = test
 *
  * ### Type-Safe Printf Wrapper
 *
 *   void safe_print(const char *label, int value) {
 *       printf(\"%s: \" FMT(value) \"\
\", label, value);
 *   }
 *
 *   void safe_print(const char *label, double value) {
 *       // Overloading doesn't work in C, but you can use FMT macro
 *       printf(\"%s: \" FMT(value) \"\
\", label, value);
 *   }
 *
 * ### Formatted Output in Arrays
 *
 *   int values[] = {10, 20, 30, 40};
 *   for (int i = 0; i < 4; i++) {
 *       printf(\"[%d] = \" FMT(values[i]) \"\
\", i, values[i]);
 *   }
 *
 * ### Mixed Type Printing
 *
 *   int count = 5;
 *   double average = 42.5;
 *   const char *label = \"Results\";
 *   int *ptr = malloc(sizeof(int));
 *
 *   printf(\"%s: \" FMT(count) \" items, avg \" FMT(average) \" (ptr: \" FMT(ptr) \")\
\",
 *          label, count, average, ptr);
 *
 * ## Advanced Usage
 *
  * ### Atomic Types
 *
 * The FMT macro supports _Atomic qualified types:
 *
 *   #include <stdatomic.h>
 *   _Atomic int x = 42;
 *   printf(FMT(x), x);
 *
 * ### Type-Safe Assertions
 *
  *   #define ASSERT_EQ(a, b) do { \\
 *       if ((a) != (b)) { \\
 *           printf(\"Assertion failed: \" FMT(a) \" != \" FMT(b) \"\
\", (a), (b)); \\
 *       } \\
 *   } while(0)
 *
 * ### Format String Building
 *
 *   const char *fmt = FMT(some_variable);
 *   printf(fmt, some_variable);
 *
 * ## Common Patterns
 *
 * ### Logging Numeric Values
 *
 *   #define LOG(var) printf(#var \": \" FMT(var) \"\
\", var)
 *
 *   int x = 42;
 *   long y = 1000000L;
 *   double z = 3.14159;
 *
 *   LOG(x);  // Prints: x: 42
 *   LOG(y);  // Prints: y: 1000000
 *   LOG(z);  // Prints: z: 3.141590
 *
 * ### Type Information
 *
 *   // Get format string without printing
 *   const char *fmt_int = FMT(42);
 *   const char *fmt_str = FMT(\"hello\");
 *
 *   printf(\"Int format: %s\
\", fmt_int);      // \"%d\"
 *   printf(\"String format: %s\
\", fmt_str);  // \"%s\"
 *
 * ### Polymorphic Output
 *
  *   void print_value(int x) {
 *       printf(\"Value: \" FMT(x) \"\
\", x);
 *   }
 *
 *   void print_value(double x) {
 *       printf(\"Value: \" FMT(x) \"\
\", x);
 *   }
 *
 *   // Note: C doesn't support function overloading,
 *   // so this pattern doesn't work as-is, but FMT helps
 *   // with the format string problem
 *
  * ## Optional: format() Function
 *
 * With FMT_IMPLEMENTATIONS defined, provides format() function:
 *
 *   #define FMT_IMPLEMENTATIONS
 *   #include \"fmt.h\"
 *
 *   char *formatted = format(\"Number: \" FMT(42), 42);
 *   printf(\"%s\
\", formatted);
 *   free(formatted);
 *
 * The format() function uses open_memstream() for dynamic string
 * building and returns allocated memory (must be freed).
 *
 * ## Important Notes
 *
 * - Requires C11 or later (_Generic support)
 * - Format string is determined at compile time
 * - Type mismatch causes compiler error if detectable
 * - Works with variable names or expressions
 * - FMT macro expands to string literal (compile-time)
 * - Parentheses required around argument: FMT(x) not FMT x
 * - Supports qualified types (const, volatile, _Atomic)
 *
 * ## Limitations
 *
 * - Doesn't work with C99 and earlier
 * - No support for custom types or structs
 * - Can't determine width or precision modifiers
 * - No support for flags like \"-\" or \"0\" padding
 * - Must use parentheses: FMT(var) not FMT var
 * - Cannot use in expressions, only as format string
 *
 * ## When to Use
 *
 * Use fmt.h when:
 * - You print many different numeric types
 * - You want to avoid format string errors
 * - You're using C11 or later
 * - You prefer type safety over flexibility
 *
 * Don't use fmt.h when:
 * - You need custom formatting (width, precision, flags)
 * - You're using C99 or earlier
 * - You need format strings at runtime
 *
 */
