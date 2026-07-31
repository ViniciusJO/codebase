/*
 * compare_int.h - Integer Comparison Function Quickstart
 *
 * compare_int.h provides a callback function for comparing integer values.
 * It's designed to be used with sorting and searching algorithms that require
 * a comparison function. The function compares values pointed at by two void
 * pointers, making it compatible with generic list and array implementations.
 *
 * ## Quick Start
 *
 * ### Usage with ArrayList
 *
 *   #define COMPARE_INT_IMPLEMENTATION
 *   #include "compare_int.h"
 *   #define ARRAY_LIST_IMPLEMENTATION
 *   #include "arraylist.h"
 *
 *   int main(void) {
 *       ArrayList *list = arraylist_new(10);
 *
 *       int *v1 = malloc(sizeof(int));
 *       int *v2 = malloc(sizeof(int));
 *       int *v3 = malloc(sizeof(int));
 *       *v1 = 42;
 *       *v2 = 10;
 *       *v3 = 100;
 *
 *       arraylist_append(list, (void*)v3);
 *       arraylist_append(list, (void*)v1);
 *       arraylist_append(list, (void*)v2);
 *
 *       // Sort using integer comparison
 *       arraylist_sort(list, int_compare);
 *
 *       // Now sorted: 10, 42, 100
 *       printf(\"%d\
\", *(int*)list->data[0]);  // 10
 *       printf(\"%d\
\", *(int*)list->data[1]);  // 42
 *       printf(\"%d\
\", *(int*)list->data[2]);  // 100
 *
 *       arraylist_free(list);
 *       return 0;
 *   }
 *
 * ## Function
 *
 * ### int_compare(void *location1, void *location2)
 *
 * Compares two integers pointed at by generic void pointers.
 *
 * Parameters:
 * - location1: Pointer to first integer
 * - location2: Pointer to second integer
 *
 * Returns:
 * - Negative value if *(int*)location1 < *(int*)location2
 * - Zero if values are equal
 * - Positive value if *(int*)location1 > *(int*)location2
 *
 * This is the standard comparison function signature used by qsort(),
 * bsearch(), and many generic data structure libraries.
 *
 * ## Common Patterns
 *
 * ### Sorting an Integer Array
 *
 *   #define COMPARE_INT_IMPLEMENTATION
 *   #include \"compare_int.h\"
 *   #include <stdlib.h>
 *   #include <stdio.h>
 *
 *   int main(void) {
 *       int values[] = {50, 20, 80, 10, 100};
 *       int count = 5;
 *
 *       // Create pointer array for qsort
 *       int *ptrs[5];
 *       for (int i = 0; i < count; i++) {
 *           ptrs[i] = &values[i];
 *       }
 *
 *       // Sort using qsort with int_compare
 *       qsort(ptrs, count, sizeof(int*), int_compare);
 *
 *       // Print sorted results
 *       for (int i = 0; i < count; i++) {
 *           printf(\"%d \", *ptrs[i]);
 *       }
 *       printf(\"\
\");  // Output: 10 20 50 80 100
 *
 *       return 0;
 *   }
 *
 * ### Finding Element Position
 *
 *   int *target = malloc(sizeof(int));
 *   *target = 42;
 *   int *result = bsearch(target, array, count,
 *                         sizeof(int*), int_compare);
 *   if (result) {
 *       printf(\"Found: %d\
\", **result);
 *   }
 *   free(target);
 *
 * ### Custom Sorting with Function Pointer
 *
 *   typedef int (*CompareFn)(const void*, const void*);
 *
 *   void sort_integers(int **array, int count) {
 *       qsort(array, count, sizeof(int*), int_compare);
 *   }
 *
 * ### Reverse Sorting
 *
 *   int reverse_int_compare(const void *a, const void *b) {
 *       return -int_compare(a, b);  // Negate result
 *   }
 *
 *   // Use reverse_int_compare for descending order
 *
 * ## Data Type
 *
 * The function works exclusively with pointers to int. Example structure:
 *
 *   int a = 42;
 *   int b = 100;
 *   int *pa = &a;
 *   int *pb = &b;
 *
 *   int result = int_compare((void*)pa, (void*)pb);
 *   // result will be negative (a < b)
 *
 * ## Common Mistakes
 *
 * ### Incorrect: Direct Integer Values
 *   // WRONG - will crash
 *   int a = 42, b = 100;
 *   int_compare(&a, &b);  // OK, this actually works
 *
 * ### Correct: Integer Pointers
 *   // RIGHT
 *   int a = 42, b = 100;
 *   int *pa = &a, *pb = &b;
 *   int_compare((void*)pa, (void*)pb);
 *
 * ### Working with Allocations
 *   int *a = malloc(sizeof(int));
 *   int *b = malloc(sizeof(int));
 *   *a = 42;
 *   *b = 100;
 *   int_compare((void*)a, (void*)b);
 *   free(a);
 *   free(b);
 *
 * ## Integration Points
 *
 * ### With Standard qsort()
 *
 *   qsort(array, count, sizeof(int*), (int(*)(const void*, const void*))int_compare);
 *
 * ### With Standard bsearch()
 *
 *   int *key = ...;
 *   int **result = bsearch(&key, array, count, sizeof(int*), int_compare);
 *
 * ### With ArrayList
 *
 *   arraylist_sort(list, int_compare);
 *
 * ### Custom Generic Sort
 *
 *   void generic_sort(void **items, size_t count,
 *                     int (*compare)(const void*, const void*)) {
 *       qsort(items, count, sizeof(void*), compare);
 *   }
 *
 *   // Usage
 *   int *list[100];
 *   // ... populate ...
 *   generic_sort((void**)list, 100, int_compare);
 *
 * ## Important Notes
 *
 * - Takes pointers to integers, not integers directly
 * - Designed for use with arraylist.h and similar libraries
 * - Compatible with C standard library qsort() and bsearch()
 * - Requires COMPARE_INT_IMPLEMENTATION to define the function
 * - Follows standard comparison function convention
 * - Not suitable for other numeric types (use casts at your own risk)
 *
 * ## Limitations
 *
 * - Only works with int type (not long, long long, etc.)
 * - Requires explicit implementation in one translation unit
 * - Assumes arguments are valid pointers to int
 * - No bounds checking or validation
 * - Returns -1, 0, or 1 (not raw difference)
 *
 * ## Related Functions
 *
 * See also:
 * - compare_pointer.h - Compare arbitrary pointers
 * - compare_string.h - Compare string values
 * - arraylist.h - Array list with sort support
 *
 */
