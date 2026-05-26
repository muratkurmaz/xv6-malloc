#define _DEFAULT_SOURCE

#include "memory_management.h"

#include <unistd.h>

/*
 * Educational sbrk-backed allocator inspired by the free-list approach used in
 * simple operating-system allocators. This implementation is intentionally small:
 * it supports aligned allocations, block reuse, and block splitting, but does not
 * perform coalescing of adjacent free blocks.
 */

static block_t free_list_sentinel;
static block_t *heapBasePointer = &free_list_sentinel;
static int heapInitialised = 0;

static void removeFromFreeList(block_t *block)
{
    block->prev->next = block->next;
    block->next->prev = block->prev;
    block->next = NULL;
    block->prev = NULL;
}

static void insertIntoFreeList(block_t *block)
{
    block_t *sentinel = (block_t *)startHeap();

    block->next = sentinel->next;
    block->prev = sentinel;
    sentinel->next->prev = block;
    sentinel->next = block;
}

static void splitBlock(block_t *block, size_t requiredSize)
{
    size_t remaining = block->size - requiredSize;

    if (remaining < MIN_SIZE) {
        return;
    }

    block_t *newBlock = (block_t *)((char *)block + requiredSize);
    newBlock->size = remaining;
    block->size = requiredSize;

    insertIntoFreeList(newBlock);
}

void initHeap(void)
{
    free_list_sentinel.size = 0;
    free_list_sentinel.next = &free_list_sentinel;
    free_list_sentinel.prev = &free_list_sentinel;
    heapBasePointer = &free_list_sentinel;
    heapInitialised = 1;
}

void *startHeap(void)
{
    if (!heapInitialised) {
        initHeap();
    }

    return heapBasePointer;
}

void *endHeap(void)
{
    return sbrk(0);
}

void *findBlock(size_t length)
{
    block_t *sentinel = (block_t *)startHeap();
    block_t *current = sentinel->next;
    block_t *best = NULL;

    while (current != sentinel) {
        if (current->size >= length && (best == NULL || current->size < best->size)) {
            best = current;
        }
        current = current->next;
    }

    if (best != NULL) {
        removeFromFreeList(best);
    }

    return best;
}

void *_malloc(size_t size)
{
    if (size == 0) {
        return NULL;
    }

    if (!heapInitialised) {
        initHeap();
    }

    size_t payloadSize = MAKE_ALIGN(size);
    size_t requiredSize = MAKE_ALIGN(sizeof(block_t) + payloadSize);

    if (requiredSize < MIN_SIZE) {
        requiredSize = MIN_SIZE;
    }

    block_t *block = (block_t *)findBlock(requiredSize);

    if (block == NULL) {
        block = (block_t *)sbrk(requiredSize);

        if (block == (void *)-1) {
            return NULL;
        }

        block->size = requiredSize;
        block->next = NULL;
        block->prev = NULL;
    } else {
        splitBlock(block, requiredSize);
    }

    return (void *)((char *)block + sizeof(block_t));
}

void _free(void *ptr)
{
    if (ptr == NULL) {
        return;
    }

    block_t *block = (block_t *)((char *)ptr - sizeof(block_t));
    insertIntoFreeList(block);
}
