#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "minheap.h"
#include "spinlock.h"





float calculatePriority(struct proc *proc){

    if (proc->runningTime == 0 ) return 0;
    if (proc->creationTime == ticks) return 0;
    return  proc->runningTime / ((float) ticks - proc->creationTime);

}

struct heap minHeap;


void MinHeap_init () {
    minHeap.capacity = NPROC;
    minHeap.currentSize = 0;
}


void heapyfy_push (int index) {
    if (index == 1 || index >= minHeap.capacity) return; // only one element or illegal value

    int parentNodeIndex = index / 2;
    HeapNode tmp;

    if (minHeap.data[parentNodeIndex].priority > minHeap.data[index].priority) { // if child has smaller priority

        // swap
        tmp = minHeap.data[parentNodeIndex];
        minHeap.data[parentNodeIndex] = minHeap.data[index];
        minHeap.data[index] = tmp;

        heapyfy_push(parentNodeIndex);
    }

}

void MinHeap_push(struct proc *proc) {
    minHeap.currentSize++;

    if (proc->state != RUNNABLE){
        panic("scheduler-heap: trying to schedule a non runnable process");
    }


    if (minHeap.currentSize >= minHeap.capacity){
        panic("scheduler-heap: too many runnable processes");

    }

    // prepare new node
    HeapNode node;
    node.priority = calculatePriority(proc);
    node.proc = proc;

    // insert to last
    minHeap.data[minHeap.currentSize] = node;

    // maintain heap recursively
    heapyfy_push(minHeap.currentSize);


}

void heapyfy_pop (int index) {
    HeapNode tmp;
    int leftIndex = 2*index;
    int rightIndex = 2*index+1;


    int minIndex;

    // locate min child
    if (rightIndex >= minHeap.currentSize) { // if rchild does not exist

        if (leftIndex >= minHeap.currentSize) return; // if lchild  also does not exist => node is a leaf
        else minIndex = leftIndex; // only one option

    }else{ // both children exist (since right exists)

        // find min-priority one
        minIndex = (minHeap.data[leftIndex].priority <= minHeap.data[rightIndex].priority) ? leftIndex : rightIndex ;
    }

    // check if invariant is preserved
    if (minHeap.data[index].priority > minHeap.data[minIndex].priority) {

        // if not so, swap and recurse
        tmp = minHeap.data[minIndex];
        minHeap.data[minIndex] = minHeap.data[index];
        minHeap.data[index] = tmp;

        heapyfy_pop(minIndex);
    }
}


int MinHeap_isEmpty(){
    int verdict = (! minHeap.currentSize);
    return verdict;
}


#define NULL 0
struct proc *MinHeap_pop(){

    struct proc *root;

    if (MinHeap_isEmpty()) return NULL;

    // pop first element
    root = minHeap.data[1].proc;

    // swap with last element
    minHeap.data[1] = minHeap.data[minHeap.currentSize];
    minHeap.currentSize--;

    // if not only element
    if (minHeap.currentSize > 1) {
        // maintain heap
        heapyfy_pop(1);
    }

    return root;

}

