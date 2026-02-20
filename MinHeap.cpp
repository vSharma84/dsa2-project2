#include "MinHeap.h"

MinHeap::MinHeap(int cap) {
    capacity = cap;
    size = 0;
    heap = new Event[capacity];
}

MinHeap::~MinHeap() {
    delete[] heap;
}

bool MinHeap::isEmpty() {
    return size == 0;
}

int MinHeap::getSize() {
    return size;
}

void MinHeap::insert(Event e) {
    if (size == capacity) return;

    heap[size] = e;
    heapifyUp(size);
    size++;
}

Event MinHeap::deleteMin() {
    Event minEvent = heap[0];
    heap[0] = heap[size - 1];
    size--;
    heapifyDown(0);
    return minEvent;
}

void MinHeap::heapifyUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;

        if (heap[index].eventTime < heap[parent].eventTime) {
            Event temp = heap[index];
            heap[index] = heap[parent];
            heap[parent] = temp;
            index = parent;
        } else {
            break;
        }
    }
}

void MinHeap::heapifyDown(int index) {
    while (true) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;

        if (left < size && heap[left].eventTime < heap[smallest].eventTime)
            smallest = left;

        if (right < size && heap[right].eventTime < heap[smallest].eventTime)
            smallest = right;

        if (smallest != index) {
            Event temp = heap[index];
            heap[index] = heap[smallest];
            heap[smallest] = temp;
            index = smallest;
        } else {
            break;
        }
    }
}
