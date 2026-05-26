# Allocator Design Notes

## Goal

The project implements a compact educational memory allocator in C. It provides `_malloc` and `_free` functions that mimic the basic behaviour of standard heap allocation while exposing the underlying memory-management ideas.

## Block Layout

Each allocation is preceded by a block header:

```c
typedef struct freeBlock {
    size_t size;
    struct freeBlock *next;
    struct freeBlock *prev;
} block_t;
```

When the block is allocated, the user receives a pointer to the payload immediately after this header.

```text
+----------------------+-------------------+
| block_t header       | user payload      |
+----------------------+-------------------+
^                      ^
block pointer          pointer returned by _malloc
```

When the block is freed, the header is inserted into the free list.

## Alignment

Allocation requests are rounded up to 8-byte boundaries using:

```c
#define MAKE_ALIGN(size) (((size) + 7) & ~7)
```

This helps ensure that returned pointers are suitable for common primitive types.

## Free List

Freed blocks are stored in a circular doubly linked list with a sentinel node. This avoids special cases for inserting or removing the first block.

## Allocation Strategy

The allocator uses a best-fit strategy:

1. Search the free list.
2. Choose the smallest free block large enough for the request.
3. Split the block if enough space remains.
4. If no free block is available, request more heap memory using `sbrk`.

## Limitations

This project is intended for learning and demonstration.

Important limitations include:

- no coalescing of adjacent free blocks
- no thread safety
- no protection against double free
- no support for `realloc`
- dependency on `sbrk`, which is mainly suitable for Unix-like educational contexts
