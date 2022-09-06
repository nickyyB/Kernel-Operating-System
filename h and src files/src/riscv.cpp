//
// Created by os on 8/10/22.
//

#include "../h/riscv.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../lib/console.h"
#include "../h/tcb.hpp"
#include "../h/printing.hpp"
#include "../h/scheduler.hpp"

void Riscv::popSppSpie()
{
    __asm__ volatile ("csrw sepc, ra");
    ms_sstatus(SSTATUS_SPP);
    __asm__ volatile ("sret");
}

void Riscv::handleSupervisorTrap() {
    //0x0000000000000009UL ECALL SUPER
    //0x0000000000000008UL ECALL USER
    //0x8000000000000001UL SOFTWARE INTERRUPT
    //0x8000000000000009UL HARDWARE INTERRUPT
    uint64 scause = r_scause();
    if(scause == 0x0000000000000008UL || scause == 0x0000000000000009UL) {
        uint64 sepc = r_sepc() + 4;
        uint64 sstatus = r_sstatus();
        uint64 arg0;
        uint64 arg1;
        void *arg2;
        char c;
        __asm__ volatile("mv %0, a0" : "=r" (arg0));
        if (arg0 == 1) { //0x01
            __asm__ volatile("mv %0, a1" : "=r" (arg1));
            __asm__ volatile("csrw sscratch, %0" : : "r" (MemoryAllocator::mem_alloc(arg1)));
        }
        else if(arg0 == 2) {//0x02
            __asm__ volatile("mv %0, a1" : "=r" (arg2));
            __asm__ volatile("csrw sscratch, %0" : : "r" (MemoryAllocator::mem_free(arg2)));
        }
        else if(arg0 == 17) {//0x11
            thread_t *myHandle;
            void (*body)(void *);
            __asm__ volatile("mv %0, a1" : "=r" (myHandle));
            __asm__ volatile("mv %0, a6" : "=r" (body));
            __asm__ volatile("mv %0, a7" : "=r" (arg2));
            *myHandle = TCB::createThread(body, arg2);
            __asm__ volatile("csrw sscratch, %0" : : "r" (myHandle));
        }
        else if(arg0==18) {//0x12
            TCB::exit();
        }
        else if(arg0==19) { //0x13
            TCB::timeSliceCounter = 0;
            TCB::dispatch();
        }
        else if(arg0==20) {//0x14
            TCB *t;
            __asm__ volatile("mv %0, a1" : "=r" (t));
            void (*f)(void *);
            __asm__ volatile("mv %0, a6" : "=r" (f));
            t->setBody(f);
            Scheduler::put(t);
        }
        else if(arg0==33){ //0x21
            sem_t *handle;
            __asm__ volatile("mv %0, a1" : "=r" (handle));
            uint value;
            __asm__ volatile("mv %0, a6" : "=r" (value));
            *handle = new KernelSemafor(handle, value);
        }
        else if(arg0==34){ //0x22
            KernelSemafor *handle;
            __asm__ volatile("mv %0, a1" : "=r" (handle));
            //int ret = handle->close();
            delete handle;
            int ret = 0;
            __asm__ volatile("csrw sscratch, %0" : : "r" (ret));
        }
        else if(arg0==35){//0x23
            KernelSemafor *handle;
            __asm__ volatile("mv %0, a1" : "=r" (handle));
            int ret = handle->wait();
            __asm__ volatile("csrw sscratch, %0" : : "r" (ret));
        }
        else if(arg0==36){//0x24
            KernelSemafor *handle;
            __asm__ volatile("mv %0, a1" : "=r" (handle));
            int ret = handle->signal();
            __asm__ volatile("csrw sscratch, %0" : : "r" (ret));
        }
        else if(arg0==65) { //0x41
            c = __getc();
            __asm__ volatile("csrw sscratch, %0" : : "r" (c));
        }
        else if(arg0==66) {//0x42
            __asm__ volatile("mv %0, a1" : "=r" (c));
            __putc(c);
        }
        else if(arg0==81){ //0x51
            sstatus = 32;
        }
        w_sstatus(sstatus);
        w_sepc(sepc);
    }
    else if(scause == 0x8000000000000001UL){
        // interrupt: yes; cause code: supervisor software interrupt (CLINT; machine timer interrupt)
        /*uint64 volatile sepc = r_sepc();
        uint64 volatile sstatus = r_sstatus();

        w_sstatus(sstatus);
        w_sepc(sepc);*/
        mc_sip(SIP_SSIP); //clear bit, first bit we handle interrupt
    }
    else if(scause == 0x8000000000000009UL){
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
        console_handler();
    }
}