#include "main.h"
#include "tasks.c"
#include "fault_hadler.c"

TASK_T tasks[3];
int main()
{
    tasks[0].handler = task_1;
    tasks[1].handler = task_2;
    tasks[2].handler = task_3;
    tasks[0].psp_top = TASK_STACK_START_1;
    tasks[1].psp_top = TASK_STACK_START_2;
    tasks[2].psp_top = TASK_STACK_START_3;

    enable_faults();
    init_schd_stack(SCHD_STACK_START);
    init_task_stack(tasks);

    // switch to sp=psp
    switch_sp_psp(tasks, current_task_id);
    __set_stk(20);
    task_1();
    for (;;)
        ;
}

void __attribute__((naked)) SysTick_Handler()
{
    // --> save current context
    // get psp value (psp now holds the current task stack top)
    __asm volatile("MRS R0,PSP");
    // push r4-r11 into current task private stack // stacking
    __asm volatile("STMDB R0!,{R4-R11}");
    // save psp
    // __asm volatile("PUSH {LR}");
    uint32_t tmp = 0x0;
    __asm("MOV %0,R0" : "=r"(tmp));
    tasks[current_task_id].psp_top = tmp;

    // --> unstacking of next task
    current_task_id++;
    current_task_id %= 3;
    // get the psp of next task
    tmp = tasks[current_task_id].psp_top;
    __asm volatile("MOV R0,%0" ::"r"(tmp));
    // unstack
    __asm volatile("LDMIA R0!,{R4-R11}");
    // update sp=psp <- of current task . i.e in R0
    __asm volatile("MSR PSP,R0");
    // --> exit
    // __asm volatile("POP {LR}");
    __asm volatile("BX LR");
}