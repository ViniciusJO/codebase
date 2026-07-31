/*
 * dyn_array.h - Dynamic Array Quickstart
 *
 * dyn_array.h provides macro-based facilities for creating type-safe dynamic
 * arrays. It offers both typed arrays (via DECLARE_DYN_ARRAY macro) and generic
 * arrays. The library handles memory management with automatic doubling of
 * capacity when needed. Choose this when you want type-specific dynamic arrays
 * without STL-like overhead.
 *
 * ## Quick Start
 *
 * ### Using Typed Dynamic Arrays
 *
 *   #include \"dyn_array.h\"
 *   #include <stdio.h>
 *
 *   // Declare a dynamic array type for integers
 *   DECLARE_DYN_ARRAY(int);
 *
 *   int main(void) {
 *       int_DynamicArray nums = {0};
 *
  *       // Initialize with capacity hint
 *       init_int_array(&nums, 10);
 *
 *       // Add elements
 *       add_int_element(&nums, 42);
 *       add_int_element(&nums, 100);
 *       add_int_element(&nums, 5);
 *
 *       // Access elements
 *       printf(\"First: %d\
\", nums.data[0]);
 *       printf(\"Size: %zu/%zu\
\", nums.size, nums.capacity);
 *
 *       // Iterate
 *       for (size_t i = 0; i < nums.size; i++) {
 *           printf(\"%d \", nums.data[i]);
 *       }
 *       printf(\"\
\");
 *
 *       // Cleanup
 *       free_int_array(&nums);
 *       return 0;
 *   }
 *
 * ## Macros and Operations
 *
 * ### DECLARE_DYN_ARRAY(type)
 *
 * Declares a complete dynamic array type and associated functions for type.
 * Generates:
 * - type_DynamicArray struct
 * - init_type_array() function
 * - resize_type_array() function
 * - add_type_element() function
 * - remove_type_element() function
 * - free_type_array() function
 *
 *   DECLARE_DYN_ARRAY(int);     // Creates int_DynamicArray type
 *   DECLARE_DYN_ARRAY(double);  // Creates double_DynamicArray type
 *   DECLARE_DYN_ARRAY(char*);   // Creates char*_DynamicArray type
 *
 * ### Generated Functions
 *
  * For each DECLARE_DYN_ARRAY(type):
 *
 * - void init_type_array(type_DynamicArray *arr, size_t initial_capacity)
 *   Initialize array with given capacity
 *
 * - void resize_type_array(type_DynamicArray *arr, size_t new_capacity)
 *   Resize array to new capacity (reallocate)
 *
 * - void add_type_element(type_DynamicArray *arr, type element)
 *   Add element to end (auto-resize if needed)
 *
 * - void remove_type_element(type_DynamicArray *arr)
 *   Remove last element
 *
 * - void free_type_array(type_DynamicArray *arr)
 *   Free memory and reset to empty state
 *
 * ### Data Structure
 *
 * Generated struct for each type:
 *
 *   typedef struct {
 *       type *data;        // Array of elements
 *       size_t size;       // Current number of elements
 *       size_t capacity;   // Maximum capacity
 *   } type_DynamicArray;
 *
 * ## Common Patterns
 *
 * ### Multiple Typed Arrays
 *
 *   #include \"dyn_array.h\"
 *
 *   DECLARE_DYN_ARRAY(int);
 *   DECLARE_DYN_ARRAY(double);
 *   DECLARE_DYN_ARRAY(char*);
 *
 *   int main(void) {
 *       int_DynamicArray ints = {0};
 *       double_DynamicArray doubles = {0};
 *       char*_DynamicArray strings = {0};
 *
 *       init_int_array(&ints, 5);
 *       init_double_array(&doubles, 10);
 *       init_char*_array(&strings, 3);
 *
 *       // Use arrays...
 *
  *       free_int_array(&ints);
 *       free_double_array(&doubles);
 *       free_char*_array(&strings);
 *   }
 *
 * ### Custom Structure Array
 *
 *   typedef struct {
 *       int id;
 *       char name[50];
 *   } Person;
 *
 *   DECLARE_DYN_ARRAY(Person);
 *
 *   int main(void) {
 *       Person_DynamicArray people = {0};
 *       init_Person_array(&people, 10);
 *
 *       Person p = {1, \"John\"};
 *       add_Person_element(&people, p);
 *
 *       free_Person_array(&people);
 *   }
 *
 * ### Generic Dynamic Arrays
 *
 * The library also provides generic functions for void* arrays:
 *
 *   GenericDynamicArray arr = {0};
 *   init_array(&arr, 10);
 *
 *   // Add elements (limited to bytes)
 *   char c = 'A';
 *   add_element(&arr, (void*)&c);
 *
 *   free_array(&arr);
 *
 * ### Generic Pointer Arrays
 *
 * For arrays of pointers:
 *
 *   GenericDynamicPointerArray ptrs = {0};
 *   init_pointer_array(&ptrs, 10);
 *
 *   int *p1 = malloc(sizeof(int));
 *   *p1 = 42;
 *   add_pointer_element(&ptrs, &p1);
 *
 *   free_pointer_array(&ptrs);
 *
 * ### Pointer Array Type
 *
 * DECLARE_DYN_POINTER_ARRAY(type) creates arrays of type* pointers:
 *
 *   typedef struct {
 *       char *name;
 *   } Item;
 *
 *   DECLARE_DYN_POINTER_ARRAY(Item);
 *
 *   Item_DynamicPointerArray items = {0};
 *   init_Item_pointer_array(&items, 10);
 *
 *   Item *item = malloc(sizeof(Item));
 *   item->name = strdup(\"example\");
 *   add_Item_pointer_element(&items, item);
 *
 *   free_Item_pointer_array(&items);
 *
 * ## Memory Management
 *
 * ### Automatic Growth
 *
 * Capacity doubles when size reaches capacity:
 *
 *   int_DynamicArray arr = {0};
 *   init_int_array(&arr, 4);
 *
 *   // Add elements 1-4: uses existing capacity
 *   for (int i = 1; i <= 4; i++) {
 *       add_int_element(&arr, i);
 *   }
 *   // size = 4, capacity = 4
 *
 *   // Add element 5: doubles to capacity 8
 *   add_int_element(&arr, 5);
 *   // size = 5, capacity = 8
 *
 * ### Manual Resize
 *
 *   int_DynamicArray arr = {0};
 *   init_int_array(&arr, 10);
 *   // ... use array ...
 *
 *   // Manually increase capacity
 *   resize_int_array(&arr, 20);
 *
 *   // Or decrease (careful about data loss)
 *   resize_int_array(&arr, 5);  // Will only keep first 5
 *
 * ### Cleanup Pattern
 *
 *   int_DynamicArray arr = {0};
 *   init_int_array(&arr, 10);
 *
 *   // Use array...
 *
 *   // Always cleanup
 *   free_int_array(&arr);
 *   // arr.data is now NULL
 *
 * ## Important Notes
 *
 * - DECLARE_DYN_ARRAY must be called at file scope or in a header
 * - Each type declaration generates separate functions
 * - Capacity always starts at 0 for generic arrays, must init
 * - resize_type_array() doesn't preserve elements above new capacity
 * - Generic arrays work with bytes; for non-byte types, use typed arrays
 * - Automatic growth uses exponential strategy (doubles capacity)
 *
 * ## Limitations
 *
 * - No bounds checking (accessing beyond size is UB)
 * - No iteration macros; use manual loops
 * - Generic arrays treat data as bytes
 * - No sorting or searching functions
 * - remove_type_element() only removes from end
 * - No random removal (implement manually if needed)
 *
 * ## When to Use
 *
 * Use dyn_array.h when you:
 * - Need type-safe dynamic arrays
 * - Prefer simple macro-based approach
 * - Want automatic memory management
 * - Work with multiple array types in same program
 *
 * Use al.h or aal.h when:
 * - You prefer struct-based containers
 * - You need more complex operations (insert, remove at position)
 * - You want a more complete API
 *
 */
