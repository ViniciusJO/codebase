

# Types of Memory Allocators
Memory allocators are system components responsible for reserving and releasing chunks of memory. While general-purpose allocators (like `malloc` in C or `new` in C++) work for most use cases, custom allocators optimize for speed, memory efficiency, and fragmentation prevention. 
---## Common Types of Allocators### 1. Linear (or Bump) Allocator* **Mechanism:** Maintains a single pointer at the start of a large memory block.* **Allocation:** The pointer simply "bumps" forward by the requested size.* **Deallocation:** Cannot free individual objects; all memory is reclaimed at once by resetting the pointer.* **Best for:** Scratchpad memory, temporary data, or short-lived program phases.
### 2. Stack Allocator* **Mechanism:** A contiguous block of memory that behaves like a stack data structure.* **Allocation:** Pushes allocations onto the stack.* **Deallocation:** Must deallocate in strict reverse order of allocation (LIFO).* **Best for:** Scoped allocations with predictable lifecycles.
### 3. Pool (or Block) Allocator* **Mechanism:** Splits a large block of memory into smaller, equal-sized chunks.* **Allocation:** Tracks available chunks using a linked list of free blocks.* **Deallocation:** Returned chunks go directly back to the free list for immediate reuse.* **Best for:** High-frequency creation/destruction of fixed-size objects (e.g., game entities, database records).
### 4. Free List Allocator* **Mechanism:** Maintains a linked list of available, varying-sized memory segments.* **Allocation:** Searches the list using strategies like First-Fit, Best-Fit, or Worst-Fit.* **Deallocation:** Returns the block to the free list and merges adjacent free blocks (coalescing).* **Best for:** General-purpose workloads with unpredictable object lifetimes and varied sizes.
### 5. Slab Allocator* **Mechanism:** Divides memory into predefined caches that store initialization-ready objects of specific types.* **Best for:** Kernel-level memory allocation (e.g., Linux kernel) to eliminate fragmentation and initialization overhead.
### 6. Buddy Allocator* **Mechanism:** Divides memory into power-of-two blocks. Splits larger blocks in half ("buddies") to satisfy small requests.* **Deallocation:** Recombines adjacent, freed buddies back into larger blocks.* **Best for:** Operating system page allocation.
---## Further Reading & Resources### Academic & Technical Deep Dives* **[Memory Management Reference](https://memorymanagement.org):** A comprehensive encyclopedia covering memory management terminology, architecture, and design patterns.
* **[The Slab Allocator (Paper)](https://usenix.org):** Jeff Bonwick's original 1994 USENIX paper introducing object-caching kernel allocation.
* **[Buddy Memory Allocation (Wikipedia)](https://wikipedia.org):** Detailed breakdown of the binary buddy allocation algorithm, mechanics, and fragmentation trade-offs.
### High-Performance Implementation Guides* **[Custom Memory Allocators (Bitsquid Blog)](https://blogspot.com):** A practical approach to writing custom allocators tailored for high-performance game engines.
* **[Game Engine Architecture (Book)](https://gameenginebook.com):** Jason Gregory's foundational text, which contains dedicated chapters on engine-specific stack, pool, and alignment-aware allocation.
### Production-Grade Allocator Source Code* **[jemalloc](https://jemalloc.net):** A general-purpose `malloc` implementation emphasizing fragmentation avoidance and scalable concurrency support.* **[mimalloc](

https://github.com

):** Microsoft's compact, free list-based allocator designed for extreme performance and predictability.* **[tcmalloc](

https://github.com

):** Google's Thread-Caching Malloc, highly optimized for multi-threaded C++ applications.

To tailor this further for your backlog, are you aiming to implement a custom allocator yourself, or are you looking to integrate an alternative general-purpose allocator into an existing project?


