//
// Created by os on 4/26/22.
//
#include "../h/printing.hpp"
#include "../h/scheduler.hpp"
#include "../h/riscv.hpp"
#include "../h/tcb.hpp"

TCB *TCB::running = nullptr;
uint64 TCB::timeSliceCounter = 0;

TCB::TCB(Body body, uint64 timeSlice, void *data) : body(body),
    stack(new uint64[STACK_SIZE] ),
    context({(uint64) &threadWrapper,(uint64) &stack[STACK_SIZE]}),
    timeSlice(timeSlice),
    finished(false),
    data(data){

    if (body != nullptr) {
        Scheduler::put(this);
    }
}

TCB *TCB::createThread(Body body, void *data) { return new TCB(body, TIME_SLICE, data); }

void TCB::dispatch() {
    TCB *old = running;
    if (!old->isFinished()) { Scheduler::put(old); }
    running = Scheduler::get();
    TCB::contextSwitch(&old->context, &running->context);
}

void TCB::exit(){
    running->setFinished(true);
    TCB::dispatch();
}

void TCB::threadWrapper() {
    Riscv::popSppSpie();
    if(running->body!= nullptr && running->data != nullptr){
        running->body(running->data);
    }
    if(running->body!= nullptr && running->data == nullptr){
        running->body(nullptr);
    }
    thread_exit();
}

void TCB::setBody(TCB::Body body) {
    this->body=body;
}

void TCB::setArg(void *arg) {
    this->data=arg;
}
