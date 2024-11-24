#ifndef __TASK_H
#define __TASK_H

/* includes */
#include <stdint.h>

/* SCB*/
#define SCB 0xE000ED00U
#define SCB_SHCSR (*(uint32_t *)((SCB) + (0x24)))
#define SCB_CFSR (*(uint32_t *)((SCB) + (0x28)))
#define SCB_HFSR (*(uint32_t *)(SCB) + (0x2C))

/* task schedular stack */
#define SRAM_START 0x20000000U
#define SRAM_SIZE ((10) * (1024))             // 10240 = 2800
#define SRAM_END ((SRAM_START) + (SRAM_SIZE)) // 0x20002800U

#define TASK_STACK_SIZE 512U
#define SCHD_STACK_SIZE 512U

#define TASK_STACK_START_1 ((SRAM_END) - (0 * (TASK_STACK_SIZE)))
#define TASK_STACK_START_2 ((SRAM_END) - (1 * (TASK_STACK_SIZE)))
#define TASK_STACK_START_3 ((SRAM_END) - (2 * (TASK_STACK_SIZE)))
#define SCHD_STACK_START ((SRAM_END) - (3 * (TASK_STACK_SIZE)))

/* systcik register */
#define STK_BASE 0xE000E010
#define STK_CTRL (*(uint32_t *)((STK_BASE) + (0x00)))
#define STK_LOAD (*(uint32_t *)((STK_BASE) + (0x04)))
#define STK_CALIB (*(uint32_t *)((STK_BASE) + (0x0C)))

/* function definatins */
int task_1();
int task_2();
int task_3();
void __set_stk(uint32_t ticks);

/* task structure*/
typedef struct task_t
{
    uint32_t psp_top;
    uint32_t (*handler)(void);
} TASK_T;

/* contants*/

uint8_t current_task_id = 0;
#endif