#ifndef MINHEAP_H
#define MINHEAP_H

#include "Event.h"

class MinHeap {
private:
    Event* heap;
    int capacity;
    int size;

    void heapifyUp(int index);
    void heapifyDown(int index);

public:
    MinHeap(int cap);
    ~MinHeap();

    void insert(Event e);
    Event deleteMin();

    bool isEmpty();
    int getSize();
};

#endif
