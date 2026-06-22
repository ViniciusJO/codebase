/*
 * compare_string.h - String Comparison Functions Quickstart
 *
 * compare_string.h provides four string comparison callback functions for use
 * with sorting and searching algorithms. Functions support both case-sensitive
 * and case-insensitive comparison, returning pointers for equality checking
 * or standard comparison results for sorting.
 *
 * ## Quick Start
 *
 * ### Basic Sorting Example
 *
 *   #define COMPARE_STRING_IMPLEMENTATION
 *   #include \"compare_string.h\"
 *   #define ARRAY_LIST_IMPLEMENTATION
 *   #include \"arraylist.h\"
 *
 *   int main(void) {
 *       ArrayList *words = arraylist_new(10);
 *
 *       arraylist_append(words, (void*)\"zebra\");
 *       arraylist_append(words, (void*)\"apple\");
 *       arraylist_append(words, (void*)\"banana\");
 *
 *       // Sort alphabetically
 *       arraylist_sort(words, string_compare);
 *
 *       for (unsigned int i = 0; i < words->length; i++) {
 *           printf(\"%s\
\", (char*)words->data[i]);
 *       }
 *       // Output: apple, banana, zebra
 *
 *       arraylist_free(words);
 *       return 0;
 *   }
 *
 * ## Functions
 *
 * ### string_equal(void *s1, void *s2)
 *
 * Returns non-zero if strings are identical, zero otherwise.
 *
 *   int equal = string_equal((void*)\"hello\", (void*)\"hello\");
 *   // Returns non-zero (true)
 *
 * ### string_compare(void *s1, void *s2)
 *
 * Standard comparison for sorting. Returns:
 * - Negative if s1 should come before s2
 * - Zero if strings are equal
 * - Positive if s1 should come after s2
 *
 *   arraylist_sort(list, string_compare);
 *
 * ### string_nocase_equal(void *s1, void *s2)
 *
 * Case-insensitive equality. Returns non-zero if strings match ignoring case.
 *
 *   int equal = string_nocase_equal((void*)\"Hello\", (void*)\"HELLO\");
 *   // Returns non-zero (true)
 *
 * ### string_nocase_compare(void *s1, void *s2)
 *
 * Case-insensitive sorting comparison. Same return values as string_compare.
 *
 *   arraylist_sort(list, string_nocase_compare);
 *
 * ## Common Patterns
 *
 * ### Sorting a List of Strings
 *
 *   #define COMPARE_STRING_IMPLEMENTATION
 *   #include \"compare_string.h\"
 *   #define ARRAY_LIST_IMPLEMENTATION
 *   #include \"arraylist.h\"
 *   #include <stdio.h>
 *
 *   int main(void) {
 *       ArrayList *names = arraylist_new(0);
 *
 *       arraylist_append(names, (void*)\"Charlie\");
 *       arraylist_append(names, (void*)\"Alice\");
 *       arraylist_append(names, (void*)\"Bob\");
 *
 *       printf(\"Unsorted:\
\");
 *       for (unsigned int i = 0; i < names->length; i++) {
 *           printf(\"  %s\
\", (char*)names->data[i]);
 *       }
 *
 *       arraylist_sort(names, string_compare);
 *
 *       printf(\"Sorted:\
\");
 *       for (unsigned int i = 0; i < names->length; i++) {
 *           printf(\"  %s\
\", (char*)names->data[i]);
 *       }
 *       // Output: Alice, Bob, Charlie
 *
 *       arraylist_free(names);
 *       return 0;
 *   }
 *
 * ### Case-Insensitive Sorting
 *
 *   ArrayList *data = arraylist_new(0);
 *   arraylist_append(data, (void*)\"ZEBRA\");
 *   arraylist_append(data, (void*)\"apple\");
 *   arraylist_append(data, (void*)\"BaNaNa\");
 *
 *   // Sort ignoring case
 *   arraylist_sort(data, string_nocase_compare);
 *
 *   // Result: apple, BaNaNa, ZEBRA (ignoring case)
 *
 * ### Finding Strings
 *
 *   ArrayList *list = arraylist_new(0);
 *   // ... populate list ...
 *
 *   int index = arraylist_index_of(list, string_equal, (void*)\"target\");
 *   if (index >= 0) {
 *       printf(\"Found at index %d\
\", index);
 *   }
 *
 * ### Case-Insensitive Search
 *
 *   // Custom wrapper for case-insensitive search
 *   typedef struct {
 *       ArrayList *list;
 *   } StringList;
 *
 *   int find_nocase(ArrayList *list, const char *target) {
 *       for (unsigned int i = 0; i < list->length; i++) {
 *           if (string_nocase_equal(list->data[i], (void*)target)) {
 *               return (int)i;
 *           }
 *       }
 *       return -1;
 *   }
 *
 * ## Advanced Examples
 *
 * ### Multi-Key Sorting
 *
 *   typedef struct {
 *       char *first_name;
 *       char *last_name;
 *   } Person;
 *
 *   int person_compare(const void *a, const void *b) {
 *       Person *p1 = (Person*)a;
 *       Person *p2 = (Person*)b;
 *
  *       // Compare by last name first
 *       int cmp = string_compare((void*)p1->last_name,
 *                               (void*)p2->last_name);
 *       if (cmp != 0) return cmp;
 *
 *       // Then by first name
 *       return string_compare((void*)p1->first_name,
 *                            (void*)p2->first_name);
 *   }
 *
 * ### Reverse Sorting
 *
 *   int reverse_string_compare(const void *a, const void *b) {
 *       return -string_compare(a, b);
 *   }
 *
  *   arraylist_sort(list, reverse_string_compare);
 *
 * ### Deduplication
 *
 *   void deduplicate_strings(ArrayList *list) {
 *       if (list->length == 0) return;
 *
 *       arraylist_sort(list, string_compare);
 *
 *       unsigned int write_pos = 1;
 *       for (unsigned int i = 1; i < list->length; i++) {
 *           if (!string_equal(list->data[i-1], list->data[i])) {
 *               list->data[write_pos++] = list->data[i];
 *           }
 *       }
 *       list->length = write_pos;
 *   }
 *
 * ### Prefix Matching
 *
 *   int find_prefix(ArrayList *list, const char *prefix) {
 *       size_t prefix_len = strlen(prefix);
 *
 *       for (unsigned int i = 0; i < list->length; i++) {
 *           if (strncmp((char*)list->data[i], prefix, prefix_len) == 0) {
 *               return (int)i;
 *           }
 *       }
 *       return -1;
 *   }
 *
 * ## Implementation Notes
 *
 * ### Case-Insensitive Algorithm
 *
 * Implemented using tolower() for each character comparison:
 *
 *   int string_nocase_compare(void *s1, void *s2) {
 *       char *p1 = (char*)s1;
 *       char *p2 = (char*)s2;
 *
 *       for (;;) {
 *           int c1 = tolower(*p1);
 *           int c2 = tolower(*p2);
 *
 *           if (c1 != c2) {
 *               if (c1 < c2) return -1;
 *               return 1;
 *           }
 *
 *           if (c1 == '\\0') break;  // End of string
 *
 *           ++p1;
 *           ++p2;
 *       }
 *
 *       return 0;
 *   }
 *
 * ## Important Notes
 *
 * - Uses standard strcmp() for case-sensitive comparison
 * - Uses tolower() for case-insensitive (portable but not locale-aware)
 * - String pointers must be valid null-terminated C strings
 * - Equality functions return non-zero for true, zero for false
 * - Comparison functions return -1, 0, or 1 (not raw difference)
 * - All functions require COMPARE_STRING_IMPLEMENTATION definition
 *
 * ## Limitations
 *
 * - Case-insensitive comparison is ASCII-only (no Unicode support)
 * - No locale awareness (doesn't handle collation rules)
 * - Requires valid null-terminated strings
 * - No partial string matching
 * - No regex or pattern matching support
 *
 * ## Related Functions
 *
 * See also:
 * - compare_int.h - Compare integer values
 * - compare_pointer.h - Compare pointer addresses
 * - arraylist.h - Array list with sort support
 *
 */
