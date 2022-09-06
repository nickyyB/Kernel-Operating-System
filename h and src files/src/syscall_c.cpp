//
// Created by os on 8/7/22.
//

#include "../h/syscall_c.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/tcb.hpp"

static uint64 syscall(uint code, ...){
    uint64 *fp = nullptr;
    uint64 ret = -1;
    __asm__ volatile ("mv %0, fp" : "=r"(fp));
    __asm__ volatile("ld a1, 0x08(fp)");
    __asm__ volatile("ld a2, 0x10(fp)");
    __asm__ volatile("ecall");
    __asm__ volatile("mv %0, a0" : "=r"(ret));
    return ret;
}

void* mem_alloc(size_t size) {
    size = count_block(size);
    return (void*) syscall(0x01, size);
}
int mem_free(void* v) {
    if(v==nullptr){ return 0;}
    return syscall(0x02, v);
}

int thread_create(thread_t *handle, void(*start_routine)(void*), void *arg){
    if(handle== nullptr) return -1;
    __asm__ volatile("mv a7, %0" : : "r" (arg));
    __asm__ volatile("mv a6, %0" : : "r" (start_routine));
    __asm__ volatile("mv a1, %0" : : "r" (handle));
    __asm__ volatile("li a0, 0x11");
    __asm__ volatile ("ecall");
    return 0;
}
int thread_exit() { return syscall(0x12);}
void thread_dispatch(){ syscall(0x13);}

void thread_start(thread_t t, void(*start_routine)(void*)){
    //syscall(0x14, t, start_routine, arg);
    __asm__ volatile("mv a6, %0" : : "r" (start_routine));
    __asm__ volatile("mv a1, %0" : : "r" (t));
    //__asm__ volatile("mv a0, %0" : : "r" (20));
    __asm__ volatile("li a0, 0x14");
    __asm__ volatile ("ecall");
}

int sem_open(sem_t *handle, unsigned init) {
    if(handle== nullptr) return -1;
    //return syscall(0x21, handle, init);
    __asm__ volatile("mv a6, %0" : : "r" (init));
    __asm__ volatile("mv a1, %0" : : "r" (handle));
    __asm__ volatile("li a0, 0x21");
    __asm__ volatile ("ecall");
    return 0;
}
int sem_close(sem_t handle){
    if(handle== nullptr) return -1;
    return syscall(0x22, handle);
}
int sem_wait(sem_t id){
    if(id== nullptr) return -1;
    return syscall(0x23, id);
}
int sem_signal(sem_t id){
    if(id == nullptr) return -1;
    return syscall(0x24, id);
}

int time_sleep(time_t t){return syscall(0x31, t);}
char getc(){return syscall(0x41);}
void putc(char c){ syscall(0x42, c);}

void userMode() { syscall(0x51);}
