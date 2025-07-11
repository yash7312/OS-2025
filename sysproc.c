#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
// #include "vm.c"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
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
sys_numvp(void){
  return myproc()->sz/PGSIZE;
}

int
sys_mmap(){
  int add_bytes;
  argint(0, &add_bytes);

  // only allocate v frames, no p frames

  if(add_bytes < 0){
    return 0;
  }

  int addr = myproc()->sz;
  myproc()->sz += add_bytes;

  if(growproc(add_bytes) < 0){
    return -1;
  }
  
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

int
hello(){
  cprintf("hello\n");
  return 0;
}

int
helloYou(){
  char *temp;
  
  argstr(0, &temp);
  cprintf("%s\n",temp);
  return 0;
}

int
sys_hellobhai(){
  int *buffer;
  argptr(0, (char**) &buffer,1);
  
  cprintf("%s\n", buffer);
  return 0;
}

int
getNumProc(){
  // return total number of active processes
  // Loop over process table looking for process to run.
  return accessNumProc();
}

int
getMaxPid(){
  // return the maximum PID amongst the PIDs of all
  // currently active processes in the system
  return accessMaxPid();
}

int
getProcInfo(){
  // getProcInfo(pid, &processInfo)
  // This system call takes as arguments an integer PID 
  // and a pointer to a structure processInfo.
  
  int pid;
  struct processInfo *p_Info;
  
  argint(0,&pid);
  argstr(1,(char**)&(p_Info));

  accessProcInfo(pid, p_Info);

  // if return 0, means its a valid process and then print all related info about it
  return 0;
}

int
setprio(){
  // void setprio(n) :: change priority of current process to n
  // change priority of process itself
  
  // max priority = 1000, can define in param.h
  
  int n;
  argint(0, &n);

  access_setPrio(n);

  return 0; // successful
}

int
getprio(){
  // return the current(?) process's priority value of process
  return access_getPrio();
}

//Definitions for part C
int 
sys_welcomeFunction(){
  int functionIP;
  if(argint(0, &functionIP) < 0)
    return -1;

  myproc()->welcome_eip = (uint)functionIP;
  return 0;

}

int 
sys_welcomeDone(){
  myproc()->tf->eip = myproc()->return_eip;
  return 0;
}