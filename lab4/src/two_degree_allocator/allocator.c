#include "allocator.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

#define PAGE_SIZE 4096

typedef struct Block {
    struct Block* next;
} Block;

typedef struct Allocator {
    Block* free_list[32];
    void* memory_start;
    size_t memory_size;
} Allocator;

static size_t next_power_of_two(size_t size) {
    size--;
    size |= size >> 1;
    size |= size >> 2;
    size |= size >> 4;
    size |= size >> 8;
    size |= size >> 16;
    return size + 1;
}

Allocator* allocator_create(void* const memory, const size_t size) {
    Allocator* allocator = (Allocator*)memory;
    if (size < sizeof(Allocator)) return NULL;

    allocator->memory_start = (char*)memory + sizeof(Allocator);
    allocator->memory_size = size - sizeof(Allocator);

    for (size_t i = 0; i < 32; i++) {
        allocator->free_list[i] = NULL;
    }

    return allocator;
}

void allocator_destroy(Allocator* const allocator) {
    for (size_t i = 0; i < 32; i++) {
        allocator->free_list[i] = NULL;
    }
}

void* allocator_alloc(Allocator* const allocator, const size_t size) {
    size_t block_size = next_power_of_two(size);
    size_t index = (size_t)log2(block_size);

    if (index >= 32) return NULL;

    Block* block = allocator->free_list[index];
    if (block) {
        allocator->free_list[index] = block->next;
        return (void*)block;
    }

    if (allocator->memory_size < block_size) return NULL;

    void* memory = allocator->memory_start;
    allocator->memory_start = (char*)allocator->memory_start + block_size;
    allocator->memory_size -= block_size;
    return memory;
}

void allocator_free(Allocator* const allocator, void* const memory) {
    if (!memory) return;

    size_t block_size = next_power_of_two((uintptr_t)memory - (uintptr_t)allocator);
    size_t index = (size_t)log2(block_size);

    if (index >= 32) return;

    Block* block = (Block*)memory;
    block->next = allocator->free_list[index];
    allocator->free_list[index] = block;
}
