/*
 * al.h - Array List Quickstart
 *
 * al.h is a header-only dynamic array library with a struct-based approach.
 * It uses macros to manipulate a lightweight array list structure containing
 * pointers to data, count, and capacity. This is ideal when you want more
 * control over the container structure and prefer explicit container types.
 *
 * ## Quick Start
 *
 * ### Basic Usage
 *
 *   #include "al.h"
 *
 *   // Define array list type
 *   al_define(int, IntArray);
 *
 *   int main(void) {
 *       IntArray nums = {0};
 *
 *       // Append elements
 *       al_append(&nums, 42);
 *       al_append(&nums, 100);
 *       al_append(&nums, 5);
 *
 *       // Access elements
 *       printf("First: %d, Last: %d\n", al_first(&nums), al_last(&nums));
 *       printf("Count: %zu / %zu\n", nums.count, nums.capacity);
 *
 *       // Iterate with foreach
 *       al_foreach(item, &nums) {
 *           printf("Value: %d\n", *item);
 *       }
 *
 *       // Cleanup
 *       al_free(&nums);
 *       return 0;
 *   }
 *
 * ## Supported Operations
 *
 * ### Type Definition
 * - al_define(type, name) - Define an array list type for a specific element type
 *   Creates a struct name with .items, .count, and .capacity fields
 *
 * ### Fundamental Operations
 * - al_append(al, item)               - Add item to end
 * - al_append_many(al, items, count)  - Add multiple items at end
 * - al_insert(al, pos, x)             - Insert at position
 * - al_insert_many(al, pos, items, count) - Insert multiple items
 * - al_reserve(al, expected_capacity) - Ensure capacity
 * - al_pop(al)                        - Remove and return last item
 * - al_pop_many(al, count)            - Pop multiple items
 * - al_remove(al, pos)                - Remove at position (shifts)
 * - al_remove_unordered(al, pos)      - Remove faster (swaps with last)
 * - al_remove_many(al, pos, count)    - Remove range
 *
 * ### Query Operations
 * - al_first(al)   - Get first item (asserts if empty)
 * - al_last(al)    - Get last item (asserts if empty)
 * - al_foreach(it, al) - Iterate over items as pointers
 *
 * ### Utility Operations
 * - al_free(al)    - Free items and reset structure
 * - al_print(al)   - Print array contents to stdout
 * - al_print_object(al) - Print with metadata
 *
 * ## Customization
 *
 * Customize with these macros before inclusion:
 *
 * - AL_INIT_CAP      - Initial capacity (default: 256)
 * - AL_ASSERT        - Custom assert (default: assert)
 * - AL_REALLOC_TYPED - Custom realloc (default: provided macro)
 * - AL_FREE          - Custom free (default: free)
 *
 * ## Common Patterns
 *
 * ### Define Multiple Types
 *
 *   al_define(int, IntArray);
 *   al_define(char*, StringArray);
 *   al_define(double, DoubleArray);
 *
 * ### String Management
 *
 *   al_define(char*, StringList);
 *   StringList names = {0};
 *
 *   al_append(&names, "Alice");
 *   al_append(&names, "Bob");
 *
 *   al_foreach(name, &names) {
 *       printf("%s\n", *name);
 *   }
 *
 * ### Structure Array
 *
 *   typedef struct {
 *       int id;
 *       char name[50];
 *   } Person;
 *
 *   al_define(Person, PersonArray);
 *   PersonArray people = {0};
 *
 *   Person p = {1, "John"};
 *   al_append(&people, p);
 *
 * ### Collecting Results
 *
 *   al_define(int, IntArray);
 *   IntArray results = {0};
 *
 *   for (int i = 1; i <= 10; i++) {
 *       if (i % 2 == 0) {
 *           al_append(&results, i);
 *       }
 *   }
 *   // results now contains all even numbers 1-10
 *
 * ## Generic Formatting
 *
 * The FMT() macro provides type-safe format specifier selection:
 *
 *   int value = 42;
 *   printf(FMT(value), value);  // Outputs: "%d"
 *
 *   double pi = 3.14;
 *   printf(FMT(pi), pi);        // Outputs: "%f"
 *
 * ## Important Notes
 *
 * - Container must be initialized to {0} or heap-allocated
 * - Positions can be negative (e.g., -1 = last position)
 * - al_foreach iterates with pointers; dereference with *
 * - al_remove_unordered() is O(1) but doesn't preserve order
 * - Capacity automatically doubles when needed
 * - Type safety is only at definition time; use consistently
 *
 * ## Limitations
 *
 * - Not truly type-safe at runtime (C limitation)
 * - No built-in sorting or searching functionality
 * - Elements are shallow-copied; manage deep copies manually
 * - The container structure is public; be careful with direct access
 *
 * ## Differences from aal.h
 *
 * al.h uses a struct-based container (you pass &array), while aal.h
 * works directly with array pointers. Choose al.h when you want explicit
 * container types and better organization.
 *
 */
