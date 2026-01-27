#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

__attribute__((no_instrument_function))
void __cyg_profile_func_enter(void *this_fn, void *call_site) {
    (void)call_site;
    printf("[hook] jumping to function at %p\n", this_fn);

    // Unsafe: jump to the function directly
    // We use inline assembly for x86_64 System V
    asm volatile(
      "movq (%%rsp), %%rax\n"      // original return address
      "movq %%rax, -8(%%rsp)\n"   // overwrite (or adjust as needed)
      "jmp *%0\n"
      :
      : "r"(this_fn+1)
      : "rax", "memory"
    );

    // asm volatile(
    //     "jmp *%0\n"
    //     :
    //     : "r"(this_fn)
    //     : "memory"
    // );
}

__attribute__((no_instrument_function))
void __cyg_profile_func_exit(void *this_fn, void *call_site) {
    (void)this_fn; (void)call_site;
    printf("[hook] function exit\n");
}

void foo(void) {
    printf("inside foo\n");
}

int main(void) {
    printf("calling foo\n");
    foo();
    printf("after foo\n");
    return 0;
}




// #define _GNU_SOURCE
// #include <stdio.h>
// #include <dlfcn.h>
// #include "nob.h"
//
// typedef struct {
//
// } Defers;
//
// // Never instrument the hook helpers themselves (prevents recursion)
// __attribute__((no_instrument_function))
// static const char *symname(void *addr, char *buf, size_t n) {
//     Dl_info info;
//     if (dladdr(addr, &info) && info.dli_sname) {
//         snprintf(buf, n, "%s", info.dli_sname);
//         return buf;
//     }
//     snprintf(buf, n, "%p", addr);
//     return buf;
// }
//
// __attribute__((no_instrument_function))
// void __cyg_profile_func_enter(void *this_fn, void *call_site) {
//     char a[256], b[256];
//     fprintf(stdout, ">> enter %s (from %s)\n",
//             symname(this_fn, a, sizeof a),
//             symname(call_site, b, sizeof b));
// }
//
// __attribute__((no_instrument_function))
// void __cyg_profile_func_exit(void *this_fn, void *call_site) {
//     char a[256], b[256];
//     fprintf(stdout, "<< exit  %s (to   %s)\n",
//             symname(this_fn, a, sizeof a),
//             symname(call_site, b, sizeof b));
// }
//
// // Demo code
// static int bar(int x) { return x + 1; }
// static int foo(int y) { return bar(y * 2); }
//
// int main(void) {
//     // printf("res=%d\n", foo(3));
//     puts("test");
//     return 0;
// }
//
