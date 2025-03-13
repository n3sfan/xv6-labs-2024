#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sysinfo.h"


uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_trace(void)
{
  struct proc *p = myproc();
  int mask; 
  argint(0, &mask); //read mask from user space

  p->traced = mask;
  return 0;
}

extern uint64 getfreemem();  // Hàm sẽ được thêm vào kernel/kalloc.c
extern uint64 getnproc(); // Hàm sẽ được thêm vào kernel/proc.c

int sys_sysinfo(void) {
  struct sysinfo si;
  si.freemem = getfreemem();
  si.nproc = getnproc();

  uint64 addr;
  argaddr(0, &addr);  // Không kiểm tra trực tiếp giá trị trả về
  if (addr == 0) {    // Kiểm tra hợp lệ
      return -1;
  }
  return copyout(myproc()->pagetable, addr, (char*)&si, sizeof(si));
}