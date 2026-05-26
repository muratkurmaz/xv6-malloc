# XV6-Style Malloc

A small C memory-allocation project implementing a custom `malloc`/`free`-style allocator using a free list and `sbrk`.

The project is inspired by the kind of heap-management mechanisms studied in operating systems courses, including the allocator design used in simple Unix-like teaching systems such as xv6.

## Overview

The allocator exposes two main functions:

```c
void *_malloc(size_t size);
void _free(void *ptr);
```

Internally, it uses:

- 8-byte alignment
- block headers stored before each allocation
- a doubly linked free list
- best-fit block selection
- block splitting when a reused free block is larger than required
- `sbrk` to request additional heap space from the process

This is an educational allocator rather than a production memory manager.

## Repository Structure

```text
.
├── include/
│   └── memory_management.h
├── src/
│   └── memory_management.c
├── examples/
│   └── demo.c
├── tests/
│   └── test_allocator.c
├── docs/
│   └── allocator-design.md
├── Makefile
├── .gitignore
└── README.md
```

## Build

```bash
make
```

This builds and runs the demo program.

## Run the Demo

```bash
make demo
```

Expected output:

```text
Hello from the custom allocator
```

## Run Local Tests

```bash
make test
```

Expected output:

```text
allocator tests passed
```

## Clean Build Files

```bash
make clean
```

## Example Usage

```c
#include "memory_management.h"

#include <stdio.h>
#include <string.h>

int main(void)
{
    char *message = _malloc(64);

    if (message == NULL) {
        return 1;
    }

    strcpy(message, "custom allocator");
    printf("%s\n", message);

    _free(message);
    return 0;
}
```

## Notes

This allocator is intentionally minimal. It is useful for understanding heap allocation, metadata headers, alignment, free-list management, and interaction with the program break through `sbrk`.

Current limitations:

- no coalescing of adjacent free blocks
- no thread safety
- no `calloc` or `realloc`
- intended for Unix-like environments where `sbrk` is available

## Author

Murat Kurmaz
