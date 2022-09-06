//
// Created by os on 6/1/22.
//
#include "../h/KernelSemafor.hpp"
#include "../h/scheduler.hpp"


KernelSemafor::KernelSemafor(sem_t *myHandle, uint initValue) {
    value = initValue;
    closed = false;
    *myHandle = this;
}

KernelSemafor::~KernelSemafor() {
    close();
}

int KernelSemafor::wait() {
    if(closed) return -1;
    --value;
    if (value<0) {
        TCB::running->setFinished(true);
        queue.addLast(TCB::running);
        TCB::dispatch();
    }
    return 0;
}

int KernelSemafor::signal() {
    if(closed) return -1;
    ++value;
    if(value<=0) {
        TCB *t = queue.removeFirst();
        if(t != nullptr) {
            t->setFinished(false);
            Scheduler::put(t);
        }
    }
    return 0;
}

void KernelSemafor::deblockAll() {
    closed = true;
    value = 0;
    TCB *t = queue.removeFirst();
    while(t!= nullptr){
        t->setFinished(false);
        Scheduler::put(t);
        t = queue.removeFirst();
    }
}

int KernelSemafor::close() {
    deblockAll();
    return 0;
}
