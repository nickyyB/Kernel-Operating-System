//
// Created by os on 8/5/22.
//

#ifndef PROJECT_BASE_MEMORYALLOCATOR_HPP
#define PROJECT_BASE_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"

typedef struct mem_block {
    size_t size;
    struct mem_block *next, *prev;
} mem_block;

void insert(mem_block **list, mem_block *newNode);
void remove(mem_block **list, mem_block *node);
size_t count_block(size_t size);

class MemoryAllocator {
public:
    static mem_block *free, *used;

    static void mem_init();
    static void*  mem_alloc(size_t size);
    static int mem_free(void* v);
    static void merge();
private:
    MemoryAllocator() = default;
};

#endif //PROJECT_BASE_MEMORYALLOCATOR_HPP
