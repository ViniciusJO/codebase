/*
 * compare_pointer.h - Generic Pointer Comparison Quickstart
 *
 * compare_pointer.h provides a callback function for comparing generic void
 * pointers by their memory addresses. It's used with sorting and searching
 * algorithms to order pointers by their location in memory, useful when you
 * need to organize pointers regardless of what they point to.
 *
 * ## Quick Start
 *
 * ### Basic Usage
 *
 *   #define COMPARE_POINTER_IMPLEMENTATION
 *   #include \"compare_pointer.h\"
 *   #include <stdlib.h>
 *   #include <stdio.h>
 *
 *   int main(void) {
 *       int a = 10, b = 20, c = 5;
 *       void *ptrs[] = { &c, &a, &b };
 *
 *       // Sort pointers by address
 *       qsort(ptrs, 3, sizeof(void*), pointer_compare);
 *
 *       // Pointers now ordered by memory address
 *       for (int i = 0; i < 3; i++) {
 *           printf(\"%p\
\", ptrs[i]);
 *       }
 *
 *       return 0;
 *   }
 *
 * ## Function
 *
 * ### int pointer_compare(void *location1, void *location2)
 *
 * Compares the memory addresses of two generic pointers.
 *
 * Parameters:
 * - location1: First pointer (memory address)
 * - location2: Second pointer (memory address)
 *
 * Returns:
 * - -1 if location1 < location2 (lower address)
 * -  0 if location1 == location2 (same address)
 * - +1 if location1 > location2 (higher address)
 *
 * ## Common Patterns
 *
 * ### Sorting Pointers by Address
 *
 *   void **ptr_array = malloc(count * sizeof(void*));
 *   // ... populate array with pointers ...
 *
 *   // Sort by address
 *   qsort(ptr_array, count, sizeof(void*), pointer_compare);
 *
 * ### Finding Pointer Ranges
 *
 *   void *base = malloc(1024);
 *   void *ptrs[] = {
 *       base + 512,
 *       base + 256,
 *       base + 768
 *   };
 *
 *   // Sort to find min/max allocation
 *   qsort(ptrs, 3, sizeof(void*), pointer_compare);
 *
 * ### Memory Region Ordering
 *
 *   struct Region {
 *       void *start;
 *       void *end;
 *   } regions[10];
 *
 *   // Create array of start pointers
 *   void *starts[10];
 *   for (int i = 0; i < 10; i++) {
 *       starts[i] = regions[i].start;
 *   }
 *
 *   // Sort regions by start address
 *   qsort(starts, 10, sizeof(void*), pointer_compare);
 *
 * ### Detecting Address Overlaps
 *
 *   int compare_and_check_overlap(void **ptrs, int count) {
 *       qsort(ptrs, count, sizeof(void*), pointer_compare);
 *
 *       for (int i = 0; i < count - 1; i++) {
 *           if (ptrs[i] == ptrs[i+1]) {
 *               printf(\"Overlapping pointers detected\
\");
 *               return 1;
 *           }
 *       }
 *       return 0;
 *   }
 *
 * ### Deduplication of Pointer Lists
 *
 *   // Remove duplicate pointers
 *   qsort(ptr_array, count, sizeof(void*), pointer_compare);
 *
 *   int unique_count = 0;
 *   for (int i = 0; i < count; i++) {
 *       if (i == 0 || ptr_array[i] != ptr_array[i-1]) {
 *           ptr_array[unique_count++] = ptr_array[i];
 *       }
 *   }
 *
 * ## Use Cases
 *
 * ### Managing Multiple Allocations
 *
 *   typedef struct {
 *       void *memory;
 *       size_t size;
 *   } Block;
 *
 *   Block blocks[100];
 *   // ... populate ...
 *
 *   // Sort blocks by address for cache efficiency
 *   void *ptrs[100];
 *   for (int i = 0; i < 100; i++) {
 *       ptrs[i] = blocks[i].memory;
 *   }
 *   qsort(ptrs, 100, sizeof(void*), pointer_compare);
 *
 * ### Pointer Deduplication in Lists
 *
 *   void *unique_pointers(void **input, int count,
 *                         int *result_count) {
 *       void **temp = malloc(count * sizeof(void*));
 *       memcpy(temp, input, count * sizeof(void*));
 *
 *       qsort(temp, count, sizeof(void*), pointer_compare);
 *
 *       int new_count = 0;
 *       for (int i = 0; i < count; i++) {
 *           if (i == 0 || temp[i] != temp[i-1]) {
 *               temp[new_count++] = temp[i];
 *           }
 *       }
 *
 *       *result_count = new_count;
 *       return temp;  // Caller must free
 *   }
 *
 * ### Pointer Pool Management
 *
 *   struct PointerPool {
 *       void **available;
 *       int available_count;
 *       void **allocated;
 *       int allocated_count;
 *   };
 *
 *   void pool_defragment(struct PointerPool *pool) {
 *       // Sort both lists by address
 *       qsort(pool->available, pool->available_count,
 *             sizeof(void*), pointer_compare);
 *       qsort(pool->allocated, pool->allocated_count,
 *             sizeof(void*), pointer_compare);
 *
 *       // Can now detect fragmentation patterns
 *   }
 *
 * ## Important Notes
 *
 * - Compares memory addresses, not data content
 * - Works with any type of pointer (generic void*)
 * - Returns normalized -1, 0, +1 (not raw address difference)
 * - Compatible with qsort() and bsearch()
 * - Useful for organizing pointers regardless of target type
 *
 * ## Limitations
 *
 * - Only compares addresses, not what they point to
 * - No type information preserved
 * - Cannot determine pointer validity
 * - Address ordering may not be semantically meaningful
 * - Requires COMPARE_POINTER_IMPLEMENTATION definition
 *
 * ## Common Misconceptions
 *
 * ### Myth: \"Sorts by pointed-at values\"
 * Fact: Sorts by memory address only. Content is irrelevant.
 *
 * ### Myth: \"Can compare any pointers\"
 * Fact: Yes, but address ordering may not be meaningful.
 *
 * ### Myth: \"Detects pointer validity\"
 * Fact: No. Invalid pointers sort \"normally\" by address.
 *
 * ## Related Functions
 *
 * See also:
 * - compare_int.h - Compare integer values
 * - compare_string.h - Compare string values
 * - arraylist.h - Array list with sort support
 *
 */
