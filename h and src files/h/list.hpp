//
// Created by marko on 20.4.22..
// Created by os on 8/14/22.
//

#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_LIST_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_LIST_HPP

#include "MemoryAllocator.hpp"

template<typename T>
class List
{
private:
    struct Elem
    {
        T *data;
        Elem *next;

        Elem(T *data, Elem *next) : data(data), next(next) {}
    };

    Elem *head, *tail;

public:
    List() : head(nullptr), tail(nullptr) {}

    List(const List<T> &) = delete;

    List<T> &operator=(const List<T> &) = delete;

    void addFirst(T *data)
    {
        //Elem *elem = new Elem(data, head);
        Elem *elem = (Elem*)MemoryAllocator::mem_alloc(count_block(sizeof(Elem)));
        elem->data = data;
        elem->next= nullptr;
        head = elem;
        if (!tail) { tail = head; }
    }

    void addLast(T *data)
    {
        //Elem *elem = new Elem(data, head);
        Elem *elem = (Elem*)MemoryAllocator::mem_alloc(count_block(sizeof(Elem)));
        elem->data = data;
        elem->next= nullptr;
        if (tail)
        {
            tail->next = elem;
            tail = elem;
        } else
        {
            head = tail = elem;
        }
    }

    T *removeFirst()
    {
        if (!head) { return nullptr; }

        Elem *elem = head;
        head = head->next;
        if (!head) { tail = nullptr; }

        T *ret = elem->data;
        //delete elem;
        MemoryAllocator::mem_free(elem);
        return ret;
    }

    T *peekFirst()
    {
        if (!head) { return nullptr; }
        return head->data;
    }

    T *removeLast()
    {
        if (!head) { return nullptr; }

        Elem *prev = nullptr;
        for (Elem *curr = head; curr && curr != tail; curr = curr->next)
        {
            prev = curr;
        }

        Elem *elem = tail;
        if (prev) { prev->next = nullptr; }
        else { head = nullptr; }
        tail = prev;

        T *ret = elem->data;
        //delete elem;
        MemoryAllocator::mem_free(elem);
        return ret;
    }

    T *peekLast()
    {
        if (!tail) { return nullptr; }
        return tail->data;
    }
};

#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_LIST_HPP
