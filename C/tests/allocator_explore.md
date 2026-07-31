# Memory Allocator Types

## Heap Allocator

The heap allocator is the standard dynamic memory allocator used by functions such as `malloc()`, `calloc()`, `realloc()`, and `free()`. It is designed for arbitrary allocation sizes and lifetimes.

### Characteristics

- Arbitrary allocation size
- Arbitrary deallocation order
- Can suffer from fragmentation
- Usually thread-safe
- General-purpose

### Complexity

- Allocation: O(1) average to O(log n), depending on implementation
- Free: O(1) average to O(log n)

### Common Uses

- General-purpose dynamic memory
- Unknown object lifetimes

---

## Page Allocator

A page allocator requests memory directly from the operating system through APIs such as `mmap()` (Linux) or `VirtualAlloc()` (Windows).

Typical page sizes include:

- 4 KiB
- 2 MiB (Huge Pages)
- 1 GiB (Huge Pages)

### Characteristics

- Provides page-aligned memory
- Backed by virtual memory
- Expensive due to system calls
- Foundation for many other allocators

---

## Arena Allocator

An arena allocator allocates many objects but frees them all simultaneously.

Objects cannot be individually deallocated.

### Characteristics

- O(1) allocation
- O(1) reset
- Very low fragmentation
- Excellent cache locality

### Common Uses

- Compilers
- AST construction
- Temporary request memory
- Game frame allocation

---

## Buffer Allocator

A buffer allocator allocates memory from a fixed-size buffer supplied by the user.

The allocator cannot grow beyond the buffer.

### Characteristics

- Fixed capacity
- No system allocations
- Common in embedded systems

---

## Stack Allocator

A stack allocator behaves like the CPU stack.

Memory must be freed in reverse order of allocation (LIFO).

### Characteristics

- O(1) allocation
- O(1) deallocation
- No fragmentation
- LIFO lifetime only

### Common Uses

- Temporary objects
- Recursive algorithms

---

## Pool (Block) Allocator

A pool allocator manages many fixed-size blocks.

Every allocation has identical size.

### Characteristics

- O(1) allocation
- O(1) deallocation
- No fragmentation
- Predictable performance

### Common Uses

- Particles
- Game entities
- Network packets

---

## Bump (Linear) Allocator

A bump allocator simply advances a pointer after every allocation.

Objects cannot be individually freed.

Resetting the allocator instantly releases all memory.

### Characteristics

- Simplest allocator
- O(1) allocation
- No fragmentation
- No metadata

Arena allocators commonly use a bump allocator internally.

---

## Batch Allocator

A batch allocator allocates memory in large chunks and then subdivides those chunks into smaller allocations.

### Characteristics

- Reduces heap allocation frequency
- Improves cache locality
- Reduces allocation overhead

### Common Uses

- ECS
- Databases
- Parsers

---

## Slab Allocator

A slab allocator stores objects of a single type inside slabs.

Each slab contains many initialized objects of identical size.

### Characteristics

- Object reuse
- Excellent cache locality
- Minimal fragmentation

### Common Uses

- Operating system kernels
- Frequently allocated kernel objects

---

## Free List Allocator

A free-list allocator maintains a collection of free memory regions.

Allocation searches this collection using strategies such as:

- First Fit
- Best Fit
- Worst Fit
- Next Fit

### Characteristics

- Flexible
- Can fragment
- Search cost depends on implementation

---

## Zone Allocator

A zone allocator groups allocations according to ownership or subsystem.

Destroying the zone releases all contained allocations simultaneously.

### Characteristics

- Similar to an arena allocator
- Organizes memory by ownership
- Simplifies lifetime management

### Common Uses

- Game engines
- Operating systems

---

## Buddy Allocator

A buddy allocator recursively splits power-of-two blocks.

When adjacent free buddies become available, they merge automatically.

### Characteristics

- Fast splitting and merging
- Predictable behavior
- Internal fragmentation due to power-of-two sizing

### Common Uses

- Kernels
- Physical page allocation

---

# Summary

| Allocator | Individual Free | Fragmentation | Speed | Typical Use |
|------------|-----------------|---------------|--------|-------------|
| Heap | Yes | High | Medium | General-purpose |
| Page | Yes | None (page granularity) | Slow | OS memory |
| Arena | No | Very Low | Very Fast | Temporary allocations |
| Buffer | Usually No | None | Very Fast | Fixed buffers |
| Stack | LIFO only | None | Extremely Fast | Nested scopes |
| Pool | Yes | None | Extremely Fast | Fixed-size objects |
| Bump | No | None | Fastest | Linear allocation |
| Batch | Depends | Low | Very Fast | Chunk allocation |
| Slab | Yes | None | Very Fast | Kernel caches |
| Free List | Yes | Medium–High | Medium | Custom allocators |
| Zone | Usually No | Very Low | Very Fast | Ownership-based memory |
| Buddy | Yes | Internal only | Fast | Kernels and page allocation |
