/*
 * vector.h - Macro-Based Vector Quickstart
 *
 * vector.h provides a minimal set of macros for working with dynamic arrays
 * (vectors) in C. It focuses on essential operations: append, pop, insert,
 * remove, and automatic memory management. The library assumes you define
 * your own vector structure with data, length, and capacity fields.
 *
 * ## Quick Start
 *
 * ### Basic Usage
 *
 *   #include <stdio.h>
 *   #include <string.h>
 *   #include \"vector.h\"
 *
  *   int main(void) {
 *       // Define a vector type
 *       typedef struct {
 *           int *data;
 *           size_t length;
 *           size_t capacity;
 *       } IntVector;
 *
 *       IntVector vec = {0};
 *
 *       // Add elements
 *       vector_append(vec, 10);
 *       vector_append(vec, 20);
 *       vector_append(vec, 30);
 *
 *       // Access elements
 *       printf(\"First: %d\
\", vec.data[0]);
 *       printf(\"Length: %zu\
\", vec.length);
 *
  *       // Remove last element
 *       vector_pop(vec);
 *
 *       // Cleanup
 *       free(vec.data);
 *       return 0;
 *   }
 *
 * ## Macro Operations
 *
 * ### Capacity Management
 *
 * - vector_grow(vector)
 *   Double the capacity. Allocates initial capacity 2 if empty.
 *
 * - vector_shrink(vector)
 *   Halve the capacity if length <= capacity/2.
 *
 * ### Adding Elements
 *
 * - vector_append(vector, element)
 *   Add element to end. Auto-grows if needed.
 *
 * - vector_unshift(vector, element)
 *   Add element to beginning. Shifts all elements forward.
 *
 * - vector_insert(vector, element, position)
 *   Insert element at specific position.
 *
 * ### Removing Elements
 *
 * - vector_pop(vector)
 *   Remove last element and shrink if needed.
 *
 * - vector_pop_return(vector, element)
 *   Remove and return last element (stores in variable).
 *
 * - vector_shift(vector)
 *   Remove first element and shift all elements back.
 *
 * - vector_shift_return(vector, element)
 *   Remove and return first element.
 *
 * - vector_remove(vector, position)
 *   Remove element at position.
 *
 * - vector_remove_return(vector, element, position)
 *   Remove and return element at position.
 *
 * ### Iteration
 *
 * - vector_foreach(vector, iterator, body)
 *   Iterate over vector elements.
 *
 * ### Display
 *
 * - vector_print(vector, format)
 *   Print vector with format specifier for each element.
 *
 * ## Common Patterns
 *
 * ### Simple Integer Vector
 *
 *   typedef struct {
 *       int *data;
 *       size_t length;
 *       size_t capacity;
 *   } IntVector;
 *
 *   int main(void) {
 *       IntVector nums = {0};
 *
 *       for (int i = 1; i <= 10; i++) {
 *           vector_append(nums, i);
 *       }
 *
 *       vector_print(&nums, \"%d\");  // Print: [1, 2, 3, ...] -> 10/capacity
 *
 *       free(nums.data);
 *       return 0;
 *   }
 *
 * ### String Vector
 *
 *   typedef struct {
 *       char **data;
 *       size_t length;
 *       size_t capacity;
 *   } StringVector;
 *
 *   StringVector words = {0};
 *
 *   vector_append(words, \"hello\");
 *   vector_append(words, \"world\");
 *
 *   vector_foreach(words, i, {
 *       printf(\"%s\
\", words.data[i]);
 *   });
 *
 * ### Queue Operations
 *
  *   // Push to end, pop from beginning
 *   vector_append(queue, item);   // Enqueue
 *   vector_shift(queue);           // Dequeue
 *
  * ### Stack Operations
 *
  *   // Push and pop from end
 *   vector_append(stack, item);   // Push
 *   vector_pop(stack);             // Pop
 *
 * ### Insert and Remove
 *
  *   IntVector vec = {0};
 *   vector_append(vec, 1);
 *   vector_append(vec, 2);
 *   vector_append(vec, 3);
 *
 *   // Insert 99 at position 1
 *   vector_insert(vec, 99, 1);
 *   // Result: [1, 99, 2, 3]
 *
  *   // Remove at position 1
 *   vector_remove(vec, 1);
 *   // Result: [1, 2, 3]
 *
 * ### Prepend (via Unshift)
 *
 *   IntVector vec = {0};
 *   vector_append(vec, 2);
 *   vector_append(vec, 3);
 *   vector_unshift(vec, 1);  // Add 1 at beginning
 *   // Result: [1, 2, 3]
 *
 * ### Extract Last Element
 *
 *   int last = 0;
 *   vector_pop_return(vec, &last);
 *   printf(\"Popped: %d\
\", last);
 *
 * ### Extract First Element
 *
 *   int first = 0;
 *   vector_shift_return(vec, &first);
 *   printf(\"Shifted: %d\
\", first);
 *
 * ## Data Structure Pattern
 *
 * All vector types should follow this pattern:
 *
 *   typedef struct {
 *       TYPE *data;        // Element array
 *       size_t length;     // Current count
 *       size_t capacity;   // Allocated capacity
 *   } NameVector;
 *
 * Where TYPE can be any C type (int, char*, struct, etc.).
 *
 * ## Memory Management
 *
 * ### Growth Strategy
 *
 * - Initial: capacity = 0, length = 0
 * - First append: capacity grows to 2
 * - When full: capacity doubles (2 -> 4 -> 8 -> 16...)
 * - When length <= capacity/2: capacity halves on pop/shift
 *
  * ### Manual Resizing
 *
 * If you need custom growth:
 *
 *   IntVector vec = {0};
 *   // Pre-allocate large capacity
 *   vec.data = malloc(1000 * sizeof(int));
 *   vec.capacity = 1000;
 *   vec.length = 0;
 *
  * ## Important Notes
 *
  * - All macros require vector to be a struct (not pointer)
 * - Length and capacity must be maintained manually
 * - Macros use realloc() which may move data in memory
 * - vector_remove() has bounds checking; invalid position breaks
 * - No type safety; you must manage types consistently
 * - Empty pointer is valid; start with {0}
 *
 * ## Limitations
 *
 * - Requires manual struct definition
 * - No generic type system (all operations are macro-based)
 * - Limited error checking (breaks on invalid bounds)
 * - vector_remove_return() marked as \"NEED TEST\" (may have bugs)
 * - Shrinking is optional; capacity doesn't automatically shrink
 * - No bounds checking in most operations
 *
 * ## Macro Warnings
 *
 * Some macros have issues documented in the source:
 *
 *   // TODO: NEED TEST
 *   #define vector_remove_return(...)
 *
 * This suggests vector_remove_return() may have undetected bugs.
 * Use vector_remove() + manual access as workaround.
 *
 * ## When to Use
 *
 * Use vector.h when:
 * - You want minimal, single-header dynamic array support
 * - You prefer explicit struct definition
 * - You need basic append/pop/insert operations
 * - You like macro-based, zero-overhead abstractions
 *
 * Use al.h or aal.h when:
 * - You want more complete, type-aware APIs
 * - You prefer automatic type generation
 * - You need more operations (find, sort, etc.)
 * - You want safer, better-tested implementations
 *
 */
