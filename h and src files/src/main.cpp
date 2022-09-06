#include "../lib/console.h"
#include "../h/MemoryAllocator.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.h"
#include "../lib/mem.h"
#include "../h/tcb.hpp"
#include "../h/userTest.hpp"

int main(){
    MemoryAllocator::mem_init();
    Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
    /*__asm__ volatile ("csrc sie, %[mask]" : : [mask] "r"(2));
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);*/

    userMode();
    //int n = 10;
    //char* niz = (char*)mem_alloc(10*sizeof(char));

    thread_t main;
    thread_create(&main, nullptr, nullptr);
    TCB::running=main;

    userMain();

    return 0;
    //KRAJ TESTA
}