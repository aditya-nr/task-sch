#include "main.h"

// Helper function to handle the fault and break into the debugger
void Debug_Break(uint32_t error_code)
{
    // Error handling logic or break into debugger
    // You can hook this to print to a debug port (USART, etc.)
    // while (1)
    //     ; // Trap the MCU in a loop for debugging
}

// Bus Fault Handler
void BusFault_Handler()
{
    volatile uint32_t CFSR = SCB_CFSR;
    volatile uint8_t bit_15_BFARVALID = (CFSR & (1 << 15)) >> 15,
                     bit_12_STKERR = (CFSR & (1 << 12)) >> 12,
                     bit_11_UNSTKERR = (CFSR & (1 << 11)) >> 11,
                     bit_10_IMPRECISERR = (CFSR & (1 << 10)) >> 10,
                     bit_9_PRECISERR = (CFSR & (1 << 9)) >> 9,
                     bit_8_IBUSERR = (CFSR & (1 << 8)) >> 8;

    if (bit_15_BFARVALID)
    {
        Debug_Break(15); // BusFault Address Register valid flag is set
    }
    else if (bit_12_STKERR)
    {
        Debug_Break(12); // Stack error during exception entry
    }
    else if (bit_11_UNSTKERR)
    {
        Debug_Break(11); // Unstacking for a return from exception failed
    }
    else if (bit_10_IMPRECISERR)
    {
        Debug_Break(10); // Imprecise data bus error
    }
    else if (bit_9_PRECISERR)
    {
        Debug_Break(9); // Precise data bus error
    }
    else if (bit_8_IBUSERR)
    {
        Debug_Break(8); // Instruction bus error
    }
    else
    {
        Debug_Break(0); // Unknown BusFault
    }
}

// Usage Fault Handler
void UsageFault_Handler()
{
    volatile uint32_t CFSR = SCB_CFSR;
    volatile uint8_t bit_25_DIVBYZERO = (CFSR & (1 << 25)) >> 25,
                     bit_24_UNALIGNED = (CFSR & (1 << 24)) >> 24,
                     bit_19_NOCP = (CFSR & (1 << 19)) >> 19,
                     bit_18_INVPC = (CFSR & (1 << 18)) >> 18,
                     bit_17_INVSTATE = (CFSR & (1 << 17)) >> 17,
                     bit_16_UNDEFINSTR = (CFSR & (1 << 16)) >> 16;

    if (bit_25_DIVBYZERO)
    {
        Debug_Break(25); // Divide by zero
    }
    else if (bit_24_UNALIGNED)
    {
        Debug_Break(24); // Unaligned memory access
    }
    else if (bit_19_NOCP)
    {
        Debug_Break(19); // Coprocessor instruction without coprocessor
    }
    else if (bit_18_INVPC)
    {
        Debug_Break(18); // Invalid PC load
    }
    else if (bit_17_INVSTATE)
    {
        Debug_Break(17); // Invalid state
    }
    else if (bit_16_UNDEFINSTR)
    {
        Debug_Break(16); // Undefined instruction
    }
    else
    {
        Debug_Break(0); // Unknown UsageFault
    }
}

// Hard Fault Handler
void HardFault_Handler()
{
    volatile uint32_t HFSR = SCB_HFSR;
    volatile uint8_t bit_31_DEBUG_VT = (HFSR & (1 << 31)) >> 31,
                     bit_30_FORCED = (HFSR & (1 << 30)) >> 30,
                     bit_1_VECTTBL = (HFSR & (1 << 1)) >> 1;

    if (bit_31_DEBUG_VT)
    {
        Debug_Break(31); // Debug event triggered HardFault
    }
    else if (bit_30_FORCED)
    {
        Debug_Break(30); // Forced HardFault
    }
    else if (bit_1_VECTTBL)
    {
        Debug_Break(1); // Vector table read error
    }
    else
    {
        Debug_Break(0); // Unknown HardFault
    }
}

// Memory Management Fault Handler
void MemManage_Handler()
{
    volatile uint32_t CFSR = SCB_CFSR;
    volatile uint8_t bit_7_MMARVALID = (CFSR & (1 << 7)) >> 7,
                     bit_4_MSTKERR = (CFSR & (1 << 4)) >> 4,
                     bit_3_MUNSTKERR = (CFSR & (1 << 3)) >> 3,
                     bit_1_DACCVIOL = (CFSR & (1 << 1)) >> 1,
                     bit_0_IACCVIOL = (CFSR & (1 << 0)) >> 0;

    if (bit_7_MMARVALID)
    {
        Debug_Break(7); // Memory Management Address Register valid flag
    }
    else if (bit_4_MSTKERR)
    {
        Debug_Break(4); // Stack error on exception entry
    }
    else if (bit_3_MUNSTKERR)
    {
        Debug_Break(3); // Unstack error on exception return
    }
    else if (bit_1_DACCVIOL)
    {
        Debug_Break(1); // Data access violation
    }
    else if (bit_0_IACCVIOL)
    {
        Debug_Break(0); // Instruction access violation
    }
    else
    {
        Debug_Break(0); // Unknown MemManage fault
    }
}
