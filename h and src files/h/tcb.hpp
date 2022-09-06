//
// Created by os on 4/26/22.
//

#ifndef PROJECT_BASE_TCB_HPP
#define PROJECT_BASE_TCB_HPP

#include "../lib/hw.h"
#include "MemoryAllocator.hpp"
#include "syscall_cpp.hpp"
#include "KernelSemafor.hpp"

class Thread;
class KernelSemafor;
class TCB {
        public:
        ~TCB() { delete[] stack; }
        bool isFinished() const { return finished; }
        void setFinished(bool value) { finished = value; }
        uint64 getTimeSlice() const { return timeSlice; }
        using Body = void (*)(void*);
        static TCB *createThread(Body body, void *data);
        //static void yield();
        static TCB *running;

        static void exit();

        void setBody(Body body);
        void setArg(void *arg);

        private:
        TCB(Body body, uint64 timeSlice, void *data);

        struct Context { uint64 ra; uint64 sp; };
        Body body;
        uint64 *stack;
        Context context;
        uint64 timeSlice;
        bool finished;
        void *data;

        friend class Riscv;
        friend class KernelSemafor;

        static void threadWrapper();
        static void contextSwitch(Context *oldContext, Context *runningContext);
        static void dispatch();

        static uint64 timeSliceCounter;

        static uint64 constexpr STACK_SIZE = DEFAULT_STACK_SIZE;
        static uint64 constexpr TIME_SLICE = DEFAULT_TIME_SLICE;
};


#endif //PROJECT_BASE_TCB_HPP
