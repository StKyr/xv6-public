
typedef struct node_t{
    struct proc *proc;
    float priority;
}HeapNode;

struct heap {
    HeapNode data[NPROC];
    int capacity;
    int currentSize;
};



void MinHeap_init ();
void MinHeap_push(struct proc *proc);
struct proc *MinHeap_pop();
int MinHeap_isEmpty();

