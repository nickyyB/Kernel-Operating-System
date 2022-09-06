//
// Created by os on 8/12/22.
//

#include "../h/syscall_cpp.hpp"

void* operator new(size_t size){ return mem_alloc(size); }
void operator delete(void* addr){ mem_free(addr); }
void operator delete[](void* addr) {mem_free(addr);}
void* operator new[](size_t size){ return mem_alloc(size); }

Thread::Thread(void (*body)(void*), void *arg) {
    //thread_create(&myHandle, body, arg);
    thread_create(&myHandle, nullptr, new Arguments(body, arg));
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::sleep(time_t time) {
    return time_sleep(time);
}

int Thread::start() {
    thread_start(myHandle, Thread::wrapper);
    return 1;
}

Thread::~Thread() noexcept {
    delete myHandle;
}

Thread::Thread() {
    //thread_create(&myHandle, nullptr, nullptr);
    thread_create(&myHandle, nullptr, new Arguments(nullptr, this));
}

void Thread::wrapper(void *arg) {
    Arguments *args = (Arguments*)arg;
    if(args->body== nullptr){
        Thread *t = (Thread*)args->arg;
        t->run();
    }
    else{
        args->body(args->arg);
    }
}

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
    return ::putc(c);
}

