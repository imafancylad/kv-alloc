# kvalloc

**kvalloc** is a memory allocator implemented from scratch in C as an educational project. Its goal is to explore low-level dynamic memory management and understand some of the mechanisms behind standard functions such as `malloc()`, `free()`, `calloc()`, and `realloc()`.

The project is being developed step by step, with an emphasis on understanding pointers, block metadata, and operating-system interfaces.

> **Status:** Work in progress. The core allocator supports small allocations through `sbrk()` and large allocations through `mmap()`. The implementation and test coverage are still being improved.

## Goals

- Understand how dynamic memory is managed in C.
- Practice pointer arithmetic and linked data structures.
- Implement a basic allocator with reusable memory blocks.
- Learn how to split and coalesce memory blocks.
- Handle errors and prevent arithmetic overflows.
- Explore system interfaces such as `sbrk()`, `mmap()`, and `munmap()`.

## Features

### Implemented

- [x] Memory allocation using `sbrk()` for smaller allocations.
- [x] Metadata describing allocated blocks.
- [x] A linked list of `sbrk()`-managed memory blocks.
- [x] Block freeing and reuse.
- [x] Splitting large free blocks.
- [x] Coalescing adjacent free blocks.
- [x] `kvcalloc()`, including zero-initialization and multiplication-overflow checking.
- [x] `kvrealloc()` for resizing allocations.
- [x] Size alignment to satisfy common C alignment requirements.
- [x] `mmap()`-based allocation for requests at or above the 128 KiB threshold.
- [x] `munmap()`-based release of `mmap()` allocations through `kvfree()`.
- [x] A defined zero-size policy: `kvalloc(0)` returns `NULL`.
- [x] Experiments covering allocation, freeing, mixed small/large allocations, multiple mappings, realloc growth/shrinking, boundary sizes, overflow checks, and `kvcalloc()` zero-initialization.

### In progress

- [ ] Test `kvrealloc()` edge cases, including `kvrealloc(NULL, size)` and `kvrealloc(ptr, 0)`.
- [ ] Expand automated tests for error paths and edge cases.
- [ ] Improve error handling and allocator robustness.
- [ ] Organize the public API in a dedicated header file.

The checklist should be updated as features are further verified and the implementation evolves.

## Project structure

```text
malloc/
├── src/
│   └── kvalloc.c
├── experiments/
│   ├── 24_mmap.c
│   ├── 25_kvalloc_mmap.c
│   ├── 26_mixed_allocations.c
│   ├── 27_multiple_mmap.c
│   ├── 28_realloc_mmap.c
│   ├── 29_realloc_shrink.c
│   ├── 30_allocation_boundaries.c
│   ├── 31_overflow_checks.c
│   └── 32_calloc_checks.c
└── README.md
```

The experiments are standalone programs used to investigate specific memory-management mechanisms and test allocator behavior. Some experiments include `src/kvalloc.c` directly to keep them simple while the project is being developed.

## Requirements

- Linux
- GCC
- Basic build tools
- Basic C knowledge is recommended

The project uses Unix/Linux-specific interfaces such as `sbrk()` and `mmap()`, so it is not intended to be portable to every platform.

## Compilation

To check that the main source file compiles:

```bash
gcc -Wall -Wextra -Wpedantic -Wconversion -Wshadow -g \
    -c src/kvalloc.c -o /tmp/kvalloc.o
```

This command creates a temporary object file and enables several useful compiler warnings. It does not produce an executable by itself.

To compile and run an experiment, for example:

```bash
gcc -Wall -Wextra -Wpedantic -Wconversion -Wshadow -g \
    experiments/27_multiple_mmap.c -o /tmp/27_multiple_mmap

/tmp/27_multiple_mmap
```

## API

The allocator uses the following function names:

```c
void *kvalloc(size_t size);
void  kvfree(void *ptr);
void *kvcalloc(size_t nmemb, size_t size);
void *kvrealloc(void *ptr, size_t size);
```

The final public interface depends on how the source code is organized. A dedicated `kvalloc.h` header has not yet been added; the current experiments include `src/kvalloc.c` directly.

### Usage example

The following example illustrates the intended API. It assumes a public `kvalloc.h` header is added and the allocator implementation is linked with the program.

```c
#include <stdio.h>
#include <string.h>
#include "kvalloc.h"

int main(void)
{
    char *text = kvalloc(32);

    if (text == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    strcpy(text, "Hello from kvalloc");
    printf("%s\n", text);

    kvfree(text);
    return 0;
}
```

Adjust the compilation command to match the actual project layout.

## Concepts explored

- **Metadata:** information stored alongside each block, such as its size and allocation status.
- **Linked list:** a data structure used to traverse and locate blocks.
- **Splitting:** dividing a large free block to make better use of available memory.
- **Coalescing:** merging adjacent free blocks to help reduce fragmentation.
- **Alignment:** ensuring memory addresses and sizes meet C type-alignment requirements.
- **`sbrk()`:** a traditional interface for changing a process's program break.
- **`mmap()` and `munmap()`:** interfaces for mapping and unmapping memory regions.
- **Overflow checking:** validating size calculations before allocating memory.

## Limitations

This project is experimental and **must not be used as a replacement for `malloc()` in production software**.

Manual memory management has many edge cases. In addition, `sbrk()` is an old interface and may conflict with other components that manage the process heap. Although the current experiments pass, the implementation still needs broader testing, especially around error handling and `kvrealloc()` edge cases, before it can be considered robust.
