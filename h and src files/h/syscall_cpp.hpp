//
// Created by os on 8/11/22.
//

#ifndef PROJECT_BASE_SYSCALL_CPP_HPP
#define PROJECT_BASE_SYSCALL_CPP_HPP

#include "syscall_c.h"
#include "tcb.hpp"


void *operator new (size_t);
void operator delete (void*);


class KernelSemafor;
typedef KernelSemafor* sem_t;

class TCB;
typedef TCB* thread_t;

class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();
    int start ();
    static void dispatch ();
    static int sleep (time_t);
protected:
    Thread ();
    virtual void run () {}
private:
    thread_t myHandle;
    static void wrapper(void*);
};


class Semaphore {
public:
    Semaphore (unsigned init = 1);
    virtual ~Semaphore ();
    int wait ();
    int signal ();
private:
    sem_t myHandle;
};

/*class PeriodicThread : public Thread {
protected:
    PeriodicThread (time_t period);
    virtual void periodicActivation () {}
};*/

class Console {
public:
    static char getc ();
    static void putc (char);
};

struct Arguments {
    void (*body)(void*);
    void* arg;
    Arguments(void (*_body)(void*), void* _arg):body(_body), arg(_arg){}
};

#endif //PROJECT_BASE_SYSCALL_CPP_HPP
