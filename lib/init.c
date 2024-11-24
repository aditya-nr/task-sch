#include <stdint.h>

#define RCC_BASE 0x40021000
#define RCC_CR (*((volatile uint32_t *)(RCC_BASE + 0x00)))
#define RCC_CFGR (*((volatile uint32_t *)(RCC_BASE + 0x04)))
#define RCC_APB2ENR (*((volatile uint32_t *)(RCC_BASE + 0x18)))

#define FLASH_BASE 0x40022000
#define FLASH_ACR (*((volatile uint32_t *)(FLASH_BASE + 0x00)))

#define HSE_STARTUP_TIMEOUT 0x0500

// Define necessary constants
#define RCC_CR_HSEON (1 << 16)
#define RCC_CR_HSERDY (1 << 17)
#define RCC_CFGR_SW_PLL (0x2 << 0)
#define RCC_CFGR_SWS_PLL (0x2 << 2)
#define RCC_CFGR_PLLSRC_HSE (1 << 16)
#define RCC_CFGR_PLLMULL9 (0x7 << 18)   // PLL multiplier set to 9
#define RCC_CFGR_HPRE_DIV1 (0x0 << 4)   // AHB clock = SYSCLK
#define RCC_CFGR_PPRE1_DIV2 (0x4 << 8)  // APB1 clock = SYSCLK / 2
#define RCC_CFGR_PPRE2_DIV1 (0x0 << 11) // APB2 clock = SYSCLK

#define FLASH_ACR_PRFTBE (1 << 4)
#define FLASH_ACR_LATENCY_2 (0x2)

// Function prototype
void SystemInit(void);

void SystemInit(void)
{
    // Enable HSE (High-Speed External clock)
    RCC_CR |= RCC_CR_HSEON;

    // Wait until HSE is ready or timeout
    uint32_t timeout = HSE_STARTUP_TIMEOUT;
    while (!(RCC_CR & RCC_CR_HSERDY) && --timeout)
        ;

    // If HSE fails to start, handle the failure (for now, we just return)
    if (!(RCC_CR & RCC_CR_HSERDY))
    {
        return;
    }

    // Set FLASH latency to 2 wait states
    FLASH_ACR |= FLASH_ACR_PRFTBE;    // Enable prefetch buffer
    FLASH_ACR &= ~0x7;                // Clear LATENCY bits
    FLASH_ACR |= FLASH_ACR_LATENCY_2; // Set latency to 2 wait states

    // Configure the PLL (HSE as PLL source, 9x multiplier -> 8MHz * 9 = 72MHz)
    RCC_CFGR |= RCC_CFGR_PLLSRC_HSE; // HSE oscillator as PLL input
    RCC_CFGR |= RCC_CFGR_PLLMULL9;   // PLL multiplication factor set to 9

    // Set AHB, APB1, APB2 clocks (prescalers)
    RCC_CFGR |= RCC_CFGR_HPRE_DIV1;  // AHB = SYSCLK
    RCC_CFGR |= RCC_CFGR_PPRE1_DIV2; // APB1 = SYSCLK / 2
    RCC_CFGR |= RCC_CFGR_PPRE2_DIV1; // APB2 = SYSCLK

    // Enable the PLL
    RCC_CR |= (1 << 24); // PLLON bit

    // Wait until PLL is ready
    while (!(RCC_CR & (1 << 25)))
        ; // PLLRDY bit

    // Select the PLL as the system clock
    RCC_CFGR &= ~(0x3 << 0);     // Clear SW bits
    RCC_CFGR |= RCC_CFGR_SW_PLL; // Select PLL as system clock

    // Wait until PLL is used as the system clock
    while (!(RCC_CFGR & RCC_CFGR_SWS_PLL))
        ;
}

void _init(void)
{
    // Empty implementation
}