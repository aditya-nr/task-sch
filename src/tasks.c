#include "main.h"
int task_1()
{
    for (;;)
        ;
}
int task_2()
{
    for (;;)
        ;
}
int task_3()
{
    for (;;)
        ;
}
/* functions */

void enable_faults()
{
    SCB_SHCSR |= (1 << 16);
    SCB_SHCSR |= (1 << 17);
    SCB_SHCSR |= (1 << 18);
}

void __set_stk(uint32_t ticks)
{
    STK_CTRL |= 0b111;
    STK_LOAD = ticks - 1;
}

__attribute__((naked)) void init_schd_stack(uint32_t schd_top_of_stack)
{
    __asm volatile("MSR MSP,%0" ::"r"(schd_top_of_stack));
    __asm volatile("BX LR");
}

void init_task_stack(TASK_T tasks[])
{
    uint32_t dummy_xpsr = 0x01000000; // set the 24 bit (T bit) as 1
    uint32_t LR = 0xFFFFFFFD;         // after intterupt exit, use PSP to unstacking and use psp as sp
    for (int i = 0; i < 3; i++)
    {
        // go to the location of psp and store dummy stack frame
        uint32_t *psp = (uint32_t *)tasks[i].psp_top;

        // store xPSR
        psp--; // full decending stack : first go to next location ,then store value
        *psp = dummy_xpsr;
        // store PC (Return Address)
        psp--;
        *psp = tasks[i].handler;
        // store LR
        psp--;
        *psp = LR;

        // now set all 12 reg with val 0
        // 12, 3, 2, 1, 0, 11, 10, 9, 8, 7, 6, 5, 4
        for (int j = 0; j <= 12; j++)
        {
            psp--;
            *psp = 0x0;
        }

        // save the psp value
        tasks[i].psp_top = (uint32_t)psp;
    }
}

void __attribute__((naked)) switch_sp_psp(TASK_T tasks[], int task_id)
{
    // set psp value to the current executing task psp
    __asm volatile("MSR PSP,%0" ::"r"(tasks[task_id].psp_top));
    // switch sp to psp
    __asm volatile("MRS R0,CONTROL");
    __asm volatile("ORR R0, #2");
    __asm volatile("MSR CONTROL,R0");

    // go back to main
    __asm volatile("BX LR");
}
