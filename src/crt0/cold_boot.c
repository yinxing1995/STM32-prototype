#include "stm32f10x.h"

/* Select systick */
static void sys_clock_set(void)
{
    /* Turn on HSE */
    RCC->CR |= (RCC_CR_HSEON);

    /* Wait for HSE to be available */
    while(!(RCC->CR & RCC_CR_HSERDY))
    {}

    /* We are running on flash so need to sync the speed, or we will fall into harfault */
    /* Enable Prefetch Buffer */
    FLASH->ACR |= FLASH_ACR_PRFTBE;

    /* Flash 2 wait state */
    FLASH->ACR &= (uint32_t)((uint32_t)~FLASH_ACR_LATENCY);
    FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_2;

    /* HSE as the source of PLL and multiply, 8Mhz * 9 = 72Mhz */
    RCC->CFGR |= (RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMULL9);

    /* AHBCLK = SYSCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
    
    /* APBCLK1 = AHBCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV1;

    /* APBCLK2 = AHBCLK */
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;

    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till PLL is ready */
    while(!(RCC->CR & RCC_CR_PLLRDY))
    {
    }
    
    RCC->CR |= RCC_CR_CSSON;

    /* Select PLL as system clock source */
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL)
    {
    }

    /* Turn off HSI */
    RCC->CR &= ~RCC_CR_HSION;

    /* Disable security clock */
    RCC->CR &= ~RCC_CR_CSSON;
}

/* Normally the default value of RCC after reset is 0x0000XX83, in which HSI is on and ready */
void sys_clock_init(void)
{
    /* Reset HSEON, RCC_CR_HSEBYP, CSSON and PLLON bits */
    RCC->CR &= ~(RCC_CR_HSEON|RCC_CR_HSEBYP|RCC_CR_CSSON|RCC_CR_PLLON);

    /* Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits */
    RCC->CFGR &= ~(RCC_CFGR_SW|RCC_CFGR_HPRE|RCC_CFGR_PPRE1|RCC_CFGR_PPRE2|RCC_CFGR_ADCPRE|RCC_CFGR_MCO);

    /* Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE/OTGFSPRE bits */
    RCC->CFGR &= ~(RCC_CFGR_PLLSRC|RCC_CFGR_PLLXTPRE|RCC_CFGR_PLLMULL|RCC_CFGR_USBPRE);

    /* Disable all interrupts and clear pending bits  */
    RCC->CIR = (RCC_CIR_LSIRDYC|RCC_CIR_LSERDYC|RCC_CIR_HSIRDYC|RCC_CIR_HSERDYC|RCC_CIR_PLLRDYC|RCC_CIR_CSSC);

    /* Set up clock */
    sys_clock_set();
}