#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"

#include <stdbool.h>
#include <float.h>

struct {
  struct spinlock lock;
  struct proc proc[NPROC];
} ptable;

static struct proc *initproc;

int nextpid = 1;
extern void forkret(void);
extern void trapret(void);

static void wakeup1(void *chan);






struct proc* intArray[NPROC];
int front = 0;
int rear = -1;
int itemCount = 0;

struct proc* peek() {
   return intArray[front];
}

bool isEmpty() {
   return itemCount == 0;
}

bool isFull() {
   return itemCount == NPROC;
}

int size() {
   return itemCount;
}

void insert(struct proc* data) {

   if(!isFull()) {

      if(rear == NPROC-1) {
         rear = -1;
      }

      intArray[++rear] = data;
      itemCount++;
   }
}

struct proc* removeData() {
   struct proc* data = intArray[front++];

   if(front == NPROC) {
      front = 0;
   }

   itemCount--;
   return data;
}









struct proc* q1[NPROC];
int front1 = 0;
int rear1 = -1;
int itemCount11 = 0;

struct proc* peek1() {
   return q1[front1];
}



bool isEmpty1() {
   return itemCount11 == 0;
}

bool isFull1() {
   return itemCount11 == NPROC;
}

int size1() {
   return itemCount11;
}


bool isIncluded1(struct proc* p){
    int i;
    for(i=0;i<size1();i++){
        if(p==q1[i])
            return true;
    }
    return false;
}

void insert1(struct proc* data) {

    if(rear1 == NPROC-1) {
        rear1 = -1;
    }

    q1[++rear1] = data;
    itemCount11++;
}

struct proc* removeData1() {
   struct proc* data = q1[front1++];

   if(front1 == NPROC) {
      front1 = 0;
   }

   itemCount11--;
   return data;
}



struct proc* q2[NPROC];
int front2 = 0;
int rear2 = -1;
int itemCount2 = 0;

struct proc* peek2() {
   return q2[front2];
}

bool isEmpty2() {
   return itemCount2 == 0;
}

bool isFull2() {
   return itemCount2 == NPROC;
}

int size2() {
   return itemCount2;
}
bool isIncluded2(struct proc* p){
    int i;
    for(i=0;i<size2();i++){
        if(p==q2[i])
            return true;
    }
    return false;
}

void insert2(struct proc* data) {

   if(!isFull2()) {

      if(rear2 == NPROC-1) {
         rear2 = -1;
      }

      q2[++rear2] = data;
      itemCount2++;
   }
}

struct proc* removeData2() {
   struct proc* data = q2[front2++];

   if(front2 == NPROC) {
      front2 = 0;
   }

   itemCount2--;
   return data;
}








struct proc* q3[NPROC];
int front3 = 0;
int rear3 = -1;
int itemCount3 = 0;

struct proc* peek3() {
   return q3[front3];
}

bool isEmpty3() {
   return itemCount3 == 0;
}

bool isFull3() {
   return itemCount3 == NPROC;
}

int size3() {
   return itemCount3;
}
bool isIncluded3(struct proc* p){
    int i;
    for(i=0;i<size3();i++){
        if(p==q3[i])
            return true;
    }
    return false;
}

void insert3(struct proc* data) {

   if(!isFull3()) {

      if(rear3 == NPROC-1) {
         rear3 = -1;
      }

      q1[++rear3] = data;
      itemCount3++;
   }
}

struct proc* removeData3() {
   struct proc* data = q3[front1++];

   if(front3 == NPROC) {
      front3 = 0;
   }

   itemCount3--;
   return data;
}

void
pinit(void)
{
  initlock(&ptable.lock, "ptable");
}

//PAGEBREAK: 32
// Look in the process table for an UNUSED proc.
// If found, change state to EMBRYO and initialize
// state required to run in the kernel.
// Otherwise return 0.
static struct proc*
allocproc(void)
{
  struct proc *p;
  char *sp;

  acquire(&ptable.lock);

  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->state == UNUSED)
      goto found;
  }
  release(&ptable.lock);
  return 0;

found:
  p->state = EMBRYO;
  p->pid = nextpid++;

  release(&ptable.lock);

  // Allocate kernel stack.
  if((p->kstack = kalloc()) == 0){
    p->state = UNUSED;
    return 0;
  }
  sp = p->kstack + KSTACKSIZE;

  // Leave room for trap frame.
  sp -= sizeof *p->tf;
  p->tf = (struct trapframe*)sp;

  // Set up new context to start executing at forkret,
  // which returns to trapret.
  sp -= 4;
  *(uint*)sp = (uint)trapret;

  sp -= sizeof *p->context;
  p->context = (struct context*)sp;
  memset(p->context, 0, sizeof *p->context);
  p->context->eip = (uint)forkret;

  p->ctime = ticks;         // start time
  p->etime = 0;             // end time
  p->rtime = 0;             // run time
  p->p_level = high;

  return p;
}

//PAGEBREAK: 32
// Set up first user process.
void
userinit(void)
{
  struct proc *p;
  extern char _binary_initcode_start[], _binary_initcode_size[];

  p = allocproc();
  p->p_level=high;
  cprintf("First user process was initiated\n");
  insert1(p);

  initproc = p;
  if((p->pgdir = setupkvm()) == 0)
    panic("userinit: out of memory?");
  inituvm(p->pgdir, _binary_initcode_start, (int)_binary_initcode_size);
  p->sz = PGSIZE;
  memset(p->tf, 0, sizeof(*p->tf));
  p->tf->cs = (SEG_UCODE << 3) | DPL_USER;
  p->tf->ds = (SEG_UDATA << 3) | DPL_USER;
  p->tf->es = p->tf->ds;
  p->tf->ss = p->tf->ds;
  p->tf->eflags = FL_IF;
  p->tf->esp = PGSIZE;
  p->tf->eip = 0;  // beginning of initcode.S

  safestrcpy(p->name, "initcode", sizeof(p->name));
  p->cwd = namei("/");

  // this assignment to p->state lets other cores
  // run this process. the acquire forces the above
  // writes to be visible, and the lock is also needed
  // because the assignment might not be atomic.
  acquire(&ptable.lock);

  p->state = RUNNABLE;
  insert(p);

  release(&ptable.lock);
}

// Grow current process's memory by n bytes.
// Return 0 on success, -1 on failure.
int
growproc(int n)
{
  uint sz;

  sz = proc->sz;
  if(n > 0){
    if((sz = allocuvm(proc->pgdir, sz, sz + n)) == 0)
      return -1;
  } else if(n < 0){
    if((sz = deallocuvm(proc->pgdir, sz, sz + n)) == 0)
      return -1;
  }
  proc->sz = sz;
  switchuvm(proc);
  return 0;
}

// Create a new process copying p as the parent.
// Sets up stack to return as if from system call.
// Caller must set state of returned proc to RUNNABLE.
int
fork(void)
{

  cprintf("Start our fork code\n");
  int i, pid;
  struct proc *np;

  // Allocate process.
  if((np = allocproc()) == 0){
    return -1;
  }

  // Copy process state from p.
  if((np->pgdir = copyuvm(proc->pgdir, proc->sz)) == 0){
    kfree(np->kstack);
    np->kstack = 0;
    np->state = UNUSED;
    return -1;
  }
  np->sz = proc->sz;
  np->parent = proc;
  *np->tf = *proc->tf;

  // Clear %eax so that fork returns 0 in the child.
  np->tf->eax = 0;

  for(i = 0; i < NOFILE; i++)
    if(proc->ofile[i])
      np->ofile[i] = filedup(proc->ofile[i]);
  np->cwd = idup(proc->cwd);

  safestrcpy(np->name, proc->name, sizeof(proc->name));

  pid = np->pid;

  acquire(&ptable.lock);

  np->state = RUNNABLE;
  insert(np);

  release(&ptable.lock);

  if(np->p_level == high){
    insert1(np);
  }else if(np->p_level == middle){
    insert2(np);
  }else{
    insert3(np);
  }
  cprintf("Using fork for creating process\n");

  return pid;
}

// Exit the current process.  Does not return.
// An exited process remains in the zombie state
// until its parent calls wait() to find out it exited.
void
exit(void)
{
  struct proc *p;
  int fd;

  if(proc == initproc)
    panic("init exiting");

  // Close all open files.
  for(fd = 0; fd < NOFILE; fd++){
    if(proc->ofile[fd]){
      fileclose(proc->ofile[fd]);
      proc->ofile[fd] = 0;
    }
  }

  begin_op();
  iput(proc->cwd);
  end_op();
  proc->cwd = 0;

  acquire(&ptable.lock);

  // Parent might be sleeping in wait().
  wakeup1(proc->parent);

  // Pass abandoned children to init.
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->parent == proc){
      p->parent = initproc;
      if(p->state == ZOMBIE)
        wakeup1(initproc);
    }
  }

  // Jump into the scheduler, never to return.
  proc->state = ZOMBIE;
  sched();
  panic("zombie exit");
}

// Wait for a child process to exit and return its pid.
// Return -1 if this process has no children.
int
wait(void)
{
  struct proc *p;
  int havekids, pid;

  acquire(&ptable.lock);
  for(;;){
    // Scan through table looking for exited children.
    havekids = 0;
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
      if(p->parent != proc)
        continue;
      havekids = 1;
      if(p->state == ZOMBIE){
        // Found one.
        pid = p->pid;
        kfree(p->kstack);
        p->kstack = 0;
        freevm(p->pgdir);
        p->pid = 0;
        p->parent = 0;
        p->name[0] = 0;
        p->killed = 0;
        p->state = UNUSED;
        release(&ptable.lock);
        return pid;
      }
    }

    // No point waiting if we don't have any children.
    if(!havekids || proc->killed){
      release(&ptable.lock);
      return -1;
    }

    // Wait for children to exit.  (See wakeup1 call in proc_exit.)
    sleep(proc, &ptable.lock);  //DOC: wait-sleep
  }
}

//PAGEBREAK: 42
// Per-CPU process scheduler.
// Each CPU calls scheduler() after setting itself up.
// Scheduler never returns.  It loops, doing:
//  - choose a process to run
//  - swtch to start running that process
//  - eventually that process transfers control
//      via swtch back to the scheduler.
void
scheduler(void)
{
  struct proc *p;

  for(;;){
    // Enable interrupts on this processor.
    sti();
    acquire(&ptable.lock);
    if(SCHEDFLAG==4){
            if(!isEmpty1()){
                //acquire(&ptable.lock);
                for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
                    if(ticks == p->ctime)
                        p->cptime = 99999;
                    else
                        p->cptime = p->rtime/(ticks - p->cptime);
                }
                //release(&ptable.lock);
                struct proc * minprocess = ptable.proc;
                float mincpt = 99999;
                //acquire(&ptable.lock);
                for(p = ptable.proc; p < &ptable.proc[NPROC] && isIncluded1(p); p++){
                    if (p->state != RUNNABLE)
                        continue;
                    if(p->cptime <= mincpt){
                        minprocess = p;
                        mincpt = p->cptime;
                    }
                }
                //release(&ptable.lock);
                if(minprocess->state == RUNNABLE){
                    proc = minprocess;
                    switchuvm(minprocess);
                    minprocess->state = RUNNING;
                    swtch(&cpu->scheduler, minprocess->context);
                    switchkvm();
                    proc = 0;
                }
                /*float min=FLT_MAX;
                float ppr;
                struct proc* q;
                acquire(&ptable.lock);
                for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
                    if(p->state != RUNNABLE)
                        continue;
                if(ticks==p->ctime){
                ppr=9999;
                }else{
                    ppr=p->rtime/(ticks-p->ctime);
                }
                if(ppr<=min){
                    q=p;
                    min=ppr;
                }
                }
                proc = q;
                switchuvm(q);
                q->state = RUNNING;
                swtch(&cpu->scheduler, q->context);
                switchkvm();
                proc=0;
                */
            }

                else if(!isEmpty2()){
               // acquire(&ptable.lock);
                for(p = ptable.proc; p < &ptable.proc[NPROC] && isIncluded2(p); p++){
                if(p->state != RUNNABLE)
                    continue;
                if(SCHEDFLAG==2){
                    if(isEmpty() || p!=peek())
                        continue;
                proc = p;
                switchuvm(p);
                p->state = RUNNING;
                removeData();
                swtch(&cpu->scheduler, p->context);
                switchkvm();
                proc=0;
        if(p->state==RUNNABLE){
            insert(p);
        }

                }
            }


            }else{
                 for(p = ptable.proc; p < &ptable.proc[NPROC] && isIncluded3(p); p++){
            if(p->state != RUNNABLE)
                continue;
            proc = p;
            switchuvm(p);
            p->state = RUNNING;
        //if(!isEmpty())
        //removeData();
            swtch(&cpu->scheduler, p->context);
            switchkvm();

        // Process is done running for now.
        // It should have changed its p->state before coming back.
            proc = 0;
                 }

            }
    }
    else if(SCHEDFLAG==3){

            //acquire(&ptable.lock);
                for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
                    if(ticks == p->ctime)
                        p->cptime = 99999;
                    else
                        p->cptime = p->rtime/(ticks - p->cptime);
                }
                //release(&ptable.lock);
                struct proc * minprocess = ptable.proc;
                float mincpt = 99999;
                //acquire(&ptable.lock);
                for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
                    if (p->state != RUNNABLE)
                        continue;
                    if(p->cptime <= mincpt){
                        minprocess = p;
                        mincpt = p->cptime;
                    }
                }
                //release(&ptable.lock);
                if(minprocess->state == RUNNABLE){
                    proc = minprocess;
                    switchuvm(minprocess);
                    minprocess->state = RUNNING;
                    swtch(&cpu->scheduler, minprocess->context);
                    switchkvm();
                    proc = 0;
                }

            /*float min=FLT_MAX;
            float ppr;
            struct proc* q;
            acquire(&ptable.lock);
         for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
            if(p->state != RUNNABLE)
                continue;
        if(ticks==p->ctime){
            ppr=9999;
        }else{
            ppr=p->rtime/(ticks-p->ctime);
            }
            if(ppr<=min){
                q=p;
                min=ppr;
            }

         }
            proc = q;
            switchuvm(q);
            q->state = RUNNING;
            swtch(&cpu->scheduler, q->context);
            switchkvm();
            proc=0;
            */
    }else{
    // Loop over process table looking for process to run.
        //acquire(&ptable.lock);
        for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
            if(p->state != RUNNABLE)
                continue;

      // Switch to chosen process.  It is the process's job
      // to release ptable.lock and then reacquire it
      // before jumping back to us.
        if(SCHEDFLAG==1 ){
        proc = p;
        switchuvm(p);
        p->state = RUNNING;
      //if(!isEmpty())
      //removeData();
        swtch(&cpu->scheduler, p->context);
        switchkvm();

      // Process is done running for now.
      // It should have changed its p->state before coming back.
        proc = 0;
        }else if(SCHEDFLAG==2){
            if(isEmpty() || p!=peek())
                continue;
            proc = p;
            switchuvm(p);
            p->state = RUNNING;
            removeData();
            swtch(&cpu->scheduler, p->context);
            switchkvm();
            proc=0;
        /*if(p->state==RUNNABLE){
            insert(p);
        }*/

            }
        }
    //release(&ptable.lock);
    }
release(&ptable.lock);
  }
}

// Enter scheduler.  Must hold only ptable.lock
// and have changed proc->state. Saves and restores
// intena because intena is a property of this
// kernel thread, not this CPU. It should
// be proc->intena and proc->ncli, but that would
// break in the few places where a lock is held but
// there's no process.
void
sched(void)
{
  int intena;

  if(!holding(&ptable.lock))
    panic("sched ptable.lock");
  if(cpu->ncli != 1)
    panic("sched locks");
  if(proc->state == RUNNING){
    panic("sched running");
    //removeData();
  }
  if(readeflags()&FL_IF)
    panic("sched interruptible");
  intena = cpu->intena;
  swtch(&proc->context, cpu->scheduler);
  cpu->intena = intena;
}

// Give up the CPU for one scheduling round.
void
yield(void)
{   cprintf("yield\n");
  acquire(&ptable.lock);  //DOC: yieldlock
  proc->state = RUNNABLE;
  //cprintf(proc->p_level);
  if(proc->p_level == high){
        cprintf("high\n");
    insert1(proc);
  }else if(proc->p_level == middle){
      insert2(proc);
   cprintf("moddle\n");
  }else{
      insert3(proc);
       cprintf("low\n");
  }
  insert(proc);
  sched();
  release(&ptable.lock);
}

// A fork child's very first scheduling by scheduler()
// will swtch here.  "Return" to user space.
void
forkret(void)
{
  static int first = 1;
  // Still holding ptable.lock from scheduler.
  release(&ptable.lock);

  if (first) {
    // Some initialization functions must be run in the context
    // of a regular process (e.g., they call sleep), and thus cannot
    // be run from main().
    first = 0;
    iinit(ROOTDEV);
    initlog(ROOTDEV);
  }

  // Return to "caller", actually trapret (see allocproc).
}

// Atomically release lock and sleep on chan.
// Reacquires lock when awakened.
void
sleep(void *chan, struct spinlock *lk)
{
  if(proc == 0)
    panic("sleep");

  if(lk == 0)
    panic("sleep without lk");

  // Must acquire ptable.lock in order to
  // change p->state and then call sched.
  // Once we hold ptable.lock, we can be
  // guaranteed that we won't miss any wakeup
  // (wakeup runs with ptable.lock locked),
  // so it's okay to release lk.
  if(lk != &ptable.lock){  //DOC: sleeplock0
    acquire(&ptable.lock);  //DOC: sleeplock1
    release(lk);
  }

  // Go to sleep.
  proc->chan = chan;
  proc->state = SLEEPING;
  sched();

  // Tidy up.
  proc->chan = 0;

  // Reacquire original lock.
  if(lk != &ptable.lock){  //DOC: sleeplock2
    release(&ptable.lock);
    acquire(lk);
  }
}

//PAGEBREAK!
// Wake up all processes sleeping on chan.
// The ptable lock must be held.
static void
wakeup1(void *chan)
{
  struct proc *p;

  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++)
    if(p->state == SLEEPING && p->chan == chan){
        p->state = RUNNABLE;
        if(p->p_level == high){
            insert1(p);
        }else if(p->p_level == middle){
            insert2(p);
        }else{
            insert3(p);
        }
        insert(p);
    }
}

// Wake up all processes sleeping on chan.
void
wakeup(void *chan)
{
  acquire(&ptable.lock);
  wakeup1(chan);
  release(&ptable.lock);
}

// Kill the process with the given pid.
// Process won't exit until it returns
// to user space (see trap in trap.c).
int
kill(int pid)
{
  struct proc *p;

  acquire(&ptable.lock);
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->pid == pid){
      p->killed = 1;
      // Wake process from sleep if necessary.
      if(p->state == SLEEPING){
        p->state = RUNNABLE;
        if(p->p_level == high){
            insert1(p);
        }else if(p->p_level == middle){
            insert2(p);
        }else{
            insert3(p);
        }
        insert(p);
      }
      release(&ptable.lock);
      return 0;
    }
  }
  release(&ptable.lock);
  return -1;
}

//PAGEBREAK: 36
// Print a process listing to console.  For debugging.
// Runs when user types ^P on console.
// No lock to avoid wedging a stuck machine further.
void
procdump(void)
{
  static char *states[] = {
  [UNUSED]    "unused",
  [EMBRYO]    "embryo",
  [SLEEPING]  "sleep ",
  [RUNNABLE]  "runble",
  [RUNNING]   "run   ",
  [ZOMBIE]    "zombie"
  };
  int i;
  struct proc *p;
  char *state;
  uint pc[10];

  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->state == UNUSED)
      continue;
    if(p->state >= 0 && p->state < NELEM(states) && states[p->state])
      state = states[p->state];
    else
      state = "???";
    cprintf("%d %s %s", p->pid, state, p->name);
    if(p->state == SLEEPING){
      getcallerpcs((uint*)p->context->ebp+2, pc);
      for(i=0; i<10 && pc[i] != 0; i++)
        cprintf(" %p", pc[i]);
    }
    cprintf("\n");
  }
}



int
getPerformanceData(int *wtime, int *rtime)
{
  struct proc *p;
  int havekids, pid;

  acquire(&ptable.lock);
  for(;;){
    // Scan through table looking for zombie children.
    havekids = 0;
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
      if(p->parent != proc)
        continue;
      havekids = 1;
      if(p->state == ZOMBIE){
        // Found one.

        // Added time field update, else same from wait system call
        *wtime = p->etime - p->ctime - p->rtime;
        *rtime = p->rtime;

        // same as wait
        pid = p->pid;
        kfree(p->kstack);
        p->kstack = 0;
        freevm(p->pgdir);
        p->state = UNUSED;
        p->pid = 0;
        p->parent = 0;
        p->name[0] = 0;
        p->killed = 0;
        release(&ptable.lock);
        return pid;
      }
    }

    // No point waiting if we don't have any children.
    if(!havekids || proc->killed){
      release(&ptable.lock);
      return -1;
    }

    // Wait for children to exit.  (See wakeup1 call in proc_exit.)
    sleep(proc, &ptable.lock);  //DOC: wait-sleep
  }
}
