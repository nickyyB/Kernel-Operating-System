//
// Created by os on 8/14/22.
//

#ifndef PROJECT_BASE_KERNELSEMAFOR_HPP
#define PROJECT_BASE_KERNELSEMAFOR_HPP
#include "syscall_cpp.hpp"
#include "list.hpp"
#include "tcb.hpp"

class KernelSemafor{
public:
    ~KernelSemafor();

    int close();
    int wait();
    int signal();

private:
    KernelSemafor(sem_t *myHandle, uint init);

    int value;
    bool closed;

    void deblockAll();

    List<TCB> queue;

    friend class Riscv;
};

#endif //PROJECT_BASE_KERNELSEMAFOR_HPP
