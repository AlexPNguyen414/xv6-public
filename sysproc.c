#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  // passing status directly did not work, so I used argint instead
  // referring to sys_kill for implementation of argint
  int status;
  if(argint(0, &status) < 0) {
    return -1;  // this should never reach I think
  }

  exit(status);     // Original: "exit();" //CS 153 Lab 1 Part a
  return 0;  // not reached
}

int
sys_wait(void)
{
  // passing address, similarly to passing exit status but using argptr instead
  // referring to argptr implementation in sys_write and sys_read in sysfile.c
  int *status;

  if(argptr(0, (char **) &status, sizeof(status)) < 0) {
    return -1;
  }

  return wait(status);  // Original: "return wait();" // TO DO: CS 153 Lab 1 Part b
}

// CS 153 Lab 1 Part c
int
sys_waitpid(void)
{
  int pid;
  int *status;
  int options;

  // passing three arguments
  if(argint(0, &pid) < 0) {
    return -1;
  }

  if(argptr(1, (char **) &status, sizeof(status)) < 0) {
    return -1;
  }

  if(argint(2, &options) < 0) {
    return -1;
  }

  return waitpid(pid, status, options);
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// hello() function for cs153 lab1
int
sys_hello(void) 
{
  hello();
  return 0;
}
