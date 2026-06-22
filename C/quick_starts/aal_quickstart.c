/*
 * aal.h - Automatic Array List Quickstart
 *
 * aal.h is a header-only, macro-based dynamic array library designed for
 * C. It provides type-safe macro interfaces for managing dynamically growing
 * arrays without requiring external dependencies. The library stores metadata
 * (count and capacity) in a hidden header before the array data.
 *
 * ## Quick Start
 *
 * ### Basic Usage
 *
 *   #include "aal.h"
 *
 *   int main(void) {
 *       int *arr = NULL;
 *
 *       // Append elements
 *       aal_append(arr, 10);
 *       aal_append(arr, 20);
 *       aal_append(arr, 30);
 *
 *       // Access elements
 *       printf("First: %d, Last: %d\n", aal_first(arr), aal_last(arr));
 *       printf("Length: %zu\n", aal_len(arr));
 *
 *       // Iterate
 *       aal_foreach(int, item, arr) {
 *           printf("Item: %d\n", *item);
 *       }
 *
 *       // Cleanup
 *       aal_free(arr);
 *       return 0;
 *   }
 *
 * ## Supported Operations
 *
 * ### Fundamental Operations
 * - aal_append(aal, x)               - Add element to end
 * - aal_append_many(aal, items, n)   - Add multiple elements at end
 * - aal_insert(aal, pos, x)          - Insert element at position
 * - aal_insert_many(aal, pos, items, n) - Insert multiple elements
 * - aal_reserve(aal, cap)            - Ensure capacity
 * - aal_pop(aal)                     - Remove and return last element
 * - aal_pop_many(aal, n)             - Pop n elements
 * - aal_remove(aal, pos)             - Remove at position (ordered)
 * - aal_remove_unordered(aal, pos)   - Remove faster (swaps with last)
 * - aal_remove_many(aal, pos, n)     - Remove range
 *
 * ### Query Operations
 * - aal_len(aal)      - Get element count
 * - aal_cap(aal)      - Get capacity
 * - aal_first(aal)    - Get first element (assert if empty)
 * - aal_last(aal)     - Get last element (assert if empty)
 * - aal_foreach(type, var, aal) - Iterate over all elements
 *
 * ### Utility Operations
 * - aal_free(aal)     - Free memory and set to NULL
 * - aal_print(aal)    - Debug print (works with integers)
 * - aal_print_object(aal) - Print with metadata
 *
 * ## Customization
 *
 * You can customize the library behavior with these macros before inclusion:
 *
 * - AAL_INIT_CAP      - Initial capacity (default: 256)
 * - AAL_ASSERT        - Custom assert function (default: assert)
 * - AAL_REALLOC       - Custom realloc (default: realloc)
 * - AAL_FREE          - Custom free (default: free)
 * - AAL_MEMCPY        - Custom memcpy (default: memcpy)
 *
 * ## Common Patterns
 *
 * ### Building a String Buffer
 *
 *   char *buffer = NULL;
 *   const char *text = "Hello";
 *   aal_append_many(buffer, (char*)text, strlen(text));
 *   aal_append(buffer, '\0');  // Null terminate
 *   // buffer is now a valid string
 *
 * ### Dynamic Integer Array
 *
 *   int *numbers = NULL;
 *   for (int i = 0; i < 100; i++) {
 *       aal_append(numbers, i * 2);
 *   }
 *   printf("Total: %zu numbers\n", aal_len(numbers));
 *   aal_free(numbers);
 *
 * ### Reversing an Array
 *
 *   int *arr = NULL;
 *   aal_append(arr, 1);
 *   aal_append(arr, 2);
 *   aal_append(arr, 3);
 *
 *   for (size_t i = 0, j = aal_len(arr)-1; i < j; i++, j--) {
 *       int tmp = arr[i];
 *       arr[i] = arr[j];
 *       arr[j] = tmp;
 *   }
 *
 * ## Important Notes
 *
 * - The array pointer can be NULL initially; operations initialize it
 * - Positions accept negative indices (e.g., -1 is the last position)
 * - aal_remove_unordered() is O(1) but doesn't preserve order
 * - Memory is allocated in powers of 2 for efficiency
 * - All assertions check for NULL pointers and bounds
 * - The library works with any type; the array type is inferred
 *
 * ## Limitations
 *
 * - No built-in sorting or searching (you manage these)
 * - No type safety at compile time (use consistently)
 * - Assertions will crash on invalid operations in debug builds
 * - The header structure is opaque and should not be accessed directly
 *
 */
