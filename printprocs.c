#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "minheap.h"
#include "spinlock.h"


extern  struct {
    struct spinlock lock;
    struct proc proc[NPROC];
} ptable;


extern struct heap minHeap;



int sys_printproc(void){

    struct proc *p;

    acquire(&ptable.lock); // need atomic access

    cprintf(" PID PPID  STATE   RTIME NSCHED PRIOR NAME\n");
    cprintf(" -----------------------------------------\n");

    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++) {
        if (p->pid == 0) continue;


        /* translate enum values to strings */
        char *state;
        switch (p->state){
            case UNUSED:    state = "UNUSED  "; break;
            case EMBRYO:    state = "EMBRYO  "; break;
            case SLEEPING:  state = "SLEEPING"; break;
            case RUNNABLE:  state = "RUNNABLE"; break;
            case RUNNING:   state = "RUNNING "; break;
            case ZOMBIE:    state = "ZOMBIE  "; break;
            default:        state = "   -    "; break;
        }


        // print PID (also fix correct spaces)
        cprintf(" %d%s",  p->pid, (p->pid < 10)? "  " : " ");

        // print PPID (if exists -for init- ) (also print correct spaces)
        if (p->parent) cprintf(" %d%s", p->parent->pid, (p->parent->pid < 10)? "   " : "  ");
        else cprintf(" -   ");

        // print STATE (spaces are fixed in switch above)
        cprintf(" %s", state);

        // print RTIME (and fix spaces)
        cprintf(" %d%s", p->runningTime, (p->runningTime<10)?"    ":(p->runningTime<100)?"   ":(p->runningTime<1000)?"  ": (p->runningTime< 10000)? " " : "");

        // print NSCHED (and fix spaces)
        cprintf(" %d%s", p->timesScheduled, (p->timesScheduled<10)?"     ":(p->timesScheduled<100)?"    ":(p->timesScheduled<1000)?"   ":(p->timesScheduled<10000)?"  ":" ");

        // print PRIOR (if applicable)
        if (p->state == RUNNABLE) {
            float priority = -1;
            int i;

            // find process-node in heap to retrieve priority
            for (i = 0; i < minHeap.currentSize+1; i++) {
                if (minHeap.data[i].proc->pid == p->pid) {
                    priority = minHeap.data[i].priority;
                    break;
                }

            }

            // actually print priority (and fix spaces)
            int intPriority = (int) (priority * 100);
            cprintf(" %d%s", intPriority, (intPriority < 10) ? "    " : (intPriority < 100) ? "   " : (intPriority < 1000) ? "  " : (intPriority < 10000) ? " " : "");

        }else{
            // not a RUNNABLE process => no priority
            cprintf(" -    ");
        }

        // print name (at last place so as not to fix spaces)
        cprintf(" %s\n", p->name);

    }

    release(&ptable.lock);

    return 0;
}