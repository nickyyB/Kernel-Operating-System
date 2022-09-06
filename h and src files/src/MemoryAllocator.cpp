//
// Created by os on 8/5/22.
//

#include "../h/MemoryAllocator.hpp"

mem_block *MemoryAllocator::free = nullptr;
mem_block *MemoryAllocator::used = nullptr;

//INITIALIZE MEM ON START
void MemoryAllocator::mem_init() {
    free = (mem_block*)HEAP_START_ADDR;
    free->size = (size_t)((char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR);
    free->prev = nullptr;
    free->next = nullptr;
    //__putc('K');
}

void *MemoryAllocator::mem_alloc(size_t number_of_blocks) {
    if(free== nullptr) return nullptr;
    //block counting before entering this function
    size_t size = number_of_blocks * MEM_BLOCK_SIZE;

    for(mem_block *curr=free; curr!= nullptr; curr=curr->next){
        if(curr->size >= size){
            remove(&free, curr);
            if((curr->size-size)>0) {
                mem_block *newNode = (mem_block *) ((char *) curr + size);
                newNode->size = (curr->size - size);
                newNode->next = nullptr;
                newNode->prev = nullptr;
                insert(&free, newNode);
            }
            curr->size = size;
            insert(&used, curr);
            return (void*)((char*)(curr) + sizeof(mem_block));
            //return (void*)((char*)curr+size)
        }
    }
    return nullptr;
}

int MemoryAllocator::mem_free(void *v) {
    if(v==nullptr) return -1; //error node not found
    if(used== nullptr) return -1; //error mem not used

    mem_block* block = (mem_block*)((char*)v - sizeof(mem_block));
    v = nullptr;
    for(mem_block* i = used; i != nullptr; i = i->next){
        if(i == block){
            remove(&used, block);
            insert(&free, block);
            merge();
            return 0;
        }
    }
    return -1;
}


void insert(mem_block **list, mem_block *newNode){
    if(newNode== nullptr) return;

    //ADD AS FIRST
    if(*list== nullptr){
        *list=newNode;
        return;
    }
    mem_block *p = *list;
    if(newNode > *list){
        mem_block *previous = nullptr;
        mem_block *curr = p;
        while(curr<newNode && curr!= nullptr){
            previous = curr;
            curr=curr->next;
        }
        //ADD AS LAST
        if(curr== nullptr){
            previous->next = newNode;
            newNode->prev = previous;
            return;
        }
        //ADD IN MIDDLE
        previous->next = newNode;
        newNode->prev = previous;
        newNode->next = curr;
        curr->prev = newNode;
        return;
    }
    else{
        p->prev = newNode;
        newNode->next = p;
        *list = newNode;
        return;
    }
}

void remove(mem_block **list, mem_block *node){
    if(node== nullptr) return;

    mem_block *curr = *list;
    mem_block *previous = nullptr;
    //FIRST ELEM
    if(curr==node){
        if(curr->next == nullptr){
            curr = curr->next;
            *list = curr;
            return;
        }
        previous = curr;
        curr = curr->next;
        previous->next = nullptr;
        curr->prev = nullptr;
        *list = curr;
        return;
    }

    while(curr!=node && curr!= nullptr){
        previous = curr;
        curr = curr->next;
    }
    //NOT FOUND
    if(curr== nullptr) return;
    previous->next = curr->next;
    curr->prev = nullptr;
    node->next = nullptr;
    return;
}


void MemoryAllocator::merge(){
    mem_block *curr = free;
    mem_block *prev = nullptr;
    //IF THERE ARE MORE THAN 2 FREE FRAGMENTS
    if(curr->next != nullptr){
        int done = 1;
        while(done==1){
            done=0;
            for(mem_block *curr=free; curr!= nullptr; curr=curr->next){
                prev = curr;
                curr = curr->next;
                if(curr== nullptr) return;
                if((char*)prev + prev->size == (char*)curr){
                    size_t newSize = prev->size + curr->size;
                    prev->next = curr->next;
                    prev->size = newSize;
                    curr->prev = curr->next= nullptr;
                    done=1;
                }
            }
        }
    }
}

size_t count_block(size_t size){
    size = size + sizeof(mem_block);
    if((size % MEM_BLOCK_SIZE)!=0) {
        size = (size/MEM_BLOCK_SIZE + 1);
    }
    else size = size/MEM_BLOCK_SIZE;
    return size;
}
