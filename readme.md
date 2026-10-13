# kvalloc

**kvalloc** is a memory allocator implemented from scratch in C as an educational project. Its goal is to explore low-level dynamic memory management and understand some of the mechanisms behind standard functions such as `malloc()`, `free()`, `calloc()`, and `realloc()`.

The project is being developed step by step, with an emphasis on understanding pointers, block metadata, and operating-system interfaces.

> **Status:** Work in progress. Some features—particularly the full integration of `mmap()` and `munmap()`—may still be under development.

## Goals

- Understand how dynamic memory is managed in C.
- Practice pointer arithmetic and linked data structures.
- Implement a basic allocator with reusable memory blocks.
- Learn how to split and coalesce memory blocks.
- Handle errors and prevent arithmetic overflows.
- Explore system interfaces such as `sbrk()`, `mmap()`, and `munmap()`.

## Features

### Implemented

- [x] Memory allocation using `sbrk()`.
- [x] Metadata describing allocated blocks.
- [x] A linked list of memory blocks.
- [x] Block freeing and reuse.
- [x] Splitting large blocks.
- [x] Coalescing adjacent free blocks.
- [x] `kvcalloc()`, including zero-initialization and overflow checking when calculating the allocation size.
- [x] `kvrealloc()` for resizing allocations.
- [x] Size alignment to satisfy common C alignment requirements.
- [x] A standalone experiment using `mmap()` and `munmap()`.

### In progress

- [ ] Integrate `mmap()` into `kvalloc()` for large allocations.
- [ ] Integrate `munmap()` into `kvfree()` to release those mappings correctly.
- [ ] Add dedicated tests for small allocations, large allocations, and edge cases.
- [ ] Continue improving error handling and allocator robustness.

This checklist should be updated as features are integrated and tested.

## Project structure

```text
malloc/
├── src/
│   └── kvalloc.c
├── experiments/
│   └── 24_mmap.c
└── README.md
```

The experiments are standalone programs used to investigate specific memory-management mechanisms before integrating them into the main allocator.

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

A warning about an auxiliary function that has not been used yet may be expected while that part of the implementation is being integrated.

## Planned API

The allocator uses the following function names:

```c
void *kvalloc(size_t size);
void  kvfree(void *ptr);
void *kvcalloc(size_t nmemb, size_t size);
void *kvrealloc(void *ptr, size_t size);
```

The final public interface depends on how the source code is organized. If a `kvalloc.h` header is added, the public function declarations should be placed there.

### Usage example

Once the public interface is available through a header, a test program could look like this:

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

This example assumes that `kvalloc.h` exists and that the allocator implementation is linked with the program. Adjust the compilation command to match the actual project layout.

## Concepts explored

- **Metadata:** information stored alongside each block, such as its size and allocation status.
- **Linked list:** a data structure used to traverse and locate blocks.
- **Splitting:** dividing a large free block to make better use of available memory.
- **Coalescing:** merging adjacent free blocks to help reduce fragmentation.
- **Alignment:** ensuring memory addresses and sizes meet C type-alignment requirements.
- **`sbrk()`:** a traditional interface for changing a process's program break.
- **`mmap()` and `munmap()`:** interfaces for mapping and unmapping memory regions.

## Limitations

This project is experimental and **must not be used as a replacement for `malloc()` in production software**.

Manual memory management has many edge cases. In addition, `sbrk()` is an old interface and may conflict with other components that manage the process heap. The implementation needs further testing before it can be considered robust.
