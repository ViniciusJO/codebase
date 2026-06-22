/*
 * arraylist_.h - Traditional Array List Quickstart
 *
 * arraylist_.h implements a traditional pointer-based array list library
 * following classic algorithm library design. It's a full implementation
 * that supports arbitrary types via void pointers, making it completely
 * generic. Use when you need a battle-tested, complete array solution.
 *
 * ## Quick Start
 *
 * ### Basic Usage (with implementation flag)
 *
 *   #define ARRAY_LIST_IMPLEMENTATION
 *   #include "arraylist.h"
 *   #include <stdio.h>
 *
 *   int main(void) {
 *       // Create array list with initial size hint
 *       ArrayList *list = arraylist_new(10);
 *
 *       // Append pointer to data
 *       int *val1 = malloc(sizeof(int));
 *       *val1 = 42;
 *       arraylist_append(list, (void*)val1);
 *
 *       int *val2 = malloc(sizeof(int));
 *       *val2 = 100;
 *       arraylist_append(list, (void*)val2);
 *
 *       // Access elements
 *       int *first = (int*)list->data[0];
 *       printf("First: %d\n", *first);
 *
 *       // Cleanup
 *       free(val1);
 *       free(val2);
 *       arraylist_free(list);
 *       return 0;
 *   }
 *
 * ## Core Operations
 *
 * ### Creation and Destruction
 * - ArrayList *arraylist_new(unsigned int length)
 *   Create with optional initial capacity hint
 * - void arraylist_free(ArrayList *arraylist)
 *   Free the list and all allocated memory
 *
 * ### Basic Operations
 * - int arraylist_append(ArrayList *al, ArrayListValue data)
 *   Add element to end; returns non-zero on success
 * - int arraylist_prepend(ArrayList *al, ArrayListValue data)
 *   Add element to beginning; returns non-zero on success
 * - void arraylist_insert(ArrayList *al, unsigned int index, ArrayListValue data)
 *   Insert at position; index must be within bounds
 * - void arraylist_remove(ArrayList *al, unsigned int index)
 *   Remove element at index
 * - void arraylist_remove_range(ArrayList *al, unsigned int index, unsigned int length)
 *   Remove range of elements starting at index
 * - void arraylist_clear(ArrayList *al)
 *   Remove all elements, keep capacity
 *
 * ### Searching
 * - int arraylist_index_of(ArrayList *al, ArrayListEqualFunc callback, ArrayListValue data)
 *   Find index using callback; returns -1 if not found
 *
 * ### Sorting
 * - void arraylist_sort(ArrayList *al, ArrayListCompareFunc compare_func)
 *   Sort using comparison function
 *
 * ## Data Structure
 *
 * The ArrayList struct contains:
 *
 *   typedef struct {
 *       ArrayListValue *data;   // Pointer array
 *       unsigned int length;    // Number of elements
 *       unsigned int _alloced;  // Capacity (private)
 *   } ArrayList;
 *
 * ## Custom Comparison Functions
 *
 * ### Comparing Values
 *
 *   int int_compare_func(ArrayListValue v1, ArrayListValue v2) {
 *       int a = *(int*)v1;
 *       int b = *(int*)v2;
 *       if (a < b) return -1;
 *       if (a > b) return 1;
 *       return 0;
 *   }
 *
 * ### Comparing Pointers for Equality
 *
 *   int string_equal_func(ArrayListValue v1, ArrayListValue v2) {
 *       return strcmp((char*)v1, (char*)v2) == 0;
 *   }
 *
 * ## Common Patterns
 *
 * ### Integer Array
 *
 *   ArrayList *nums = arraylist_new(0);
 *   for (int i = 1; i <= 100; i++) {
 *       int *n = malloc(sizeof(int));
 *       *n = i;
 *       arraylist_append(nums, (void*)n);
 *   }
 *
 *   // Process and cleanup
 *   for (unsigned int i = 0; i < nums->length; i++) {
 *       free(nums->data[i]);
 *   }
 *   arraylist_free(nums);
 *
 * ### String Collection
 *
 *   ArrayList *names = arraylist_new(0);
 *
 *   arraylist_append(names, (void*)"Alice");
 *   arraylist_append(names, (void*)"Bob");
 *   arraylist_append(names, (void*)"Charlie");
 *
 *   // Iterate
 *   for (unsigned int i = 0; i < names->length; i++) {
 *       printf("%s\n", (char*)names->data[i]);
 *   }
 *
 *   arraylist_free(names);  // Only frees the list, not strings
 *
 * ### Structure Array
 *
 *   typedef struct {
 *       int id;
 *       char name[50];
 *   } Person;
 *
 *   ArrayList *people = arraylist_new(10);
 *
 *   Person *p1 = malloc(sizeof(Person));
 *   p1->id = 1;
 *   strcpy(p1->name, "John");
 *   arraylist_append(people, (void*)p1);
 *
 * ### Finding Elements
 *
 *   int find_string(ArrayListValue v1, ArrayListValue v2) {
 *       return strcmp((char*)v1, (char*)v2) == 0;
 *   }
 *
 *   ArrayList *list = arraylist_new(0);
 *   // ... populate list ...
 *
 *   int index = arraylist_index_of(list, find_string, (void*)"target");
 *   if (index >= 0) {
 *       printf("Found at index %d\n", index);
 *   }
 *
 * ### Sorting Integers
 *
 *   int compare_ints(ArrayListValue a, ArrayListValue b) {
 *       int ia = *(int*)a;
 *       int ib = *(int*)b;
 *       return (ia > ib) - (ia < ib);
 *   }
 *
 *   ArrayList *nums = arraylist_new(0);
 *   // ... append numbers ...
 *   arraylist_sort(nums, compare_ints);
 *
 * ## Memory Management
 *
 * ### Important Notes
 *
 * - ArrayList stores void pointers, not data itself
 * - You must allocate memory for non-pointer types
 * - You must free memory in array before freeing the list
 * - The list doubles capacity automatically when needed
 * - Initial capacity hint: 0 = default (~5), or specify custom
 *
 * ### Cleanup Pattern
 *
 *   // If elements are dynamically allocated
 *   for (unsigned int i = 0; i < list->length; i++) {
 *       free(list->data[i]);
 *   }
 *   arraylist_free(list);
 *
 *   // If elements are static strings or extern data
 *   arraylist_free(list);  // Just free the list
 *
 * ## Error Handling
 *
 * - append() and prepend() return 0 on allocation failure
 * - insert() returns 0 on failure
 * - Other operations silently fail if index is out of bounds
 * - Use bounds checking before accessing data directly
 *
 * ## Important Notes
 *
 * - Uses quicksort for sorting (not stable)
 * - Capacity managed automatically; doubles on demand
 * - No assertions; check return values explicitly
 * - Generic void pointers; no type safety
 * - Widely portable and battle-tested design
 *
 * ## Limitations
 *
 * - No type safety (requires careful casting)
 * - Manual memory management for elements
 * - Append/insert can fail silently (check return values)
 * - No bounds checking on direct array access
 * - No iteration macros (use manual loops)
 *
 * ## Comparison with al.h and aal.h
 *
 * - arraylist_.h: Traditional, pointer-based, fully generic
 * - al.h: Struct-based, type-aware via macros, modern C style
 * - aal.h: Array-based (not pointer-based), hidden header, lightweight
 *
 */
