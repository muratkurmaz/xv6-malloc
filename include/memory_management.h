#ifndef MEMORY_MANAGEMENT_H
#define MEMORY_MANAGEMENT_H

#include <stddef.h>

#define ALIGNMENT 8UL
#define MAKE_ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))

/*
 * Free-list block header.
 *
 * The same header is stored immediately before each allocated payload.
 * When a block is free, the next/prev pointers link it into the free list.
 */
typedef struct freeBlock {
    size_t size;
    struct freeBlock *next;
    struct freeBlock *prev;
} block_t;

#define MIN_SIZE MAKE_ALIGN(sizeof(block_t))

/* Supporting allocator functions */
void initHeap(void);
void *startHeap(void);
void *endHeap(void);
void *findBlock(size_t length);

/* Public allocation interface */
void *_malloc(size_t size);
void _free(void *ptr);

#endif /* MEMORY_MANAGEMENT_H */
