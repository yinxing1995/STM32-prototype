#include "stm32f10x_rtc.h"
#include "stm32f10x_pwr.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_exti.h"
#include "stm32f10x.h"
#include "cold_boot.h"

#define RTC_LSB_MASK     ((uint32_t)0x0000FFFF)  /*!< RTC LSB Mask */
#define PRLH_MSB_MASK    ((uint32_t)0x000F0000)  /*!< RTC Prescaler MSB Mask */

#define     RTC_WAIT_LAST_WRITE()   while (!(RTC->CRL & RTC_FLAG_RTOFF))


void rtc_init()
{
    /* Enable clock for APB1 */
    RCC->APB1ENR |= RCC_APB1Periph_PWR|RCC_APB1Periph_BKP;

    /* Enable access to backup registers and RTC */
    PWR->CR |= PWR_CR_DBP;

    /* LSE on */
    RCC->BDCR |= RCC_BDCR_LSEON;

    /* Wait till LSE is ready */
    while(!(RCC->BDCR & RCC_BDCR_LSERDY))
    {}

    /* Select LSE as the source of RTC */
    RCC->BDCR |= RCC_BDCR_RTCSEL_LSE;

    __asm("dmb 0");

    /* RTC enable */
    RCC->BDCR |= RCC_BDCR_RTCEN;

    /* RTC wait for sync after clock source set */
    RTC->CRL &= ~(uint16_t)RTC_FLAG_RSF;

    /* Loop until RSF flag is set */
    while (!(RTC->CRL & RTC_FLAG_RSF))
    {}

    /* Enter config mode */
    RTC->CRL |= RTC_CRL_CNF;

    /* Wait for ongoing write operation */
    RTC_WAIT_LAST_WRITE();

    /* Set prescaler, here LSE is 32.768khz */
    RTC->PRLH = (32767 & PRLH_MSB_MASK) >> 16;
    RTC->PRLL = (32767 & RTC_LSB_MASK);

    RTC_WAIT_LAST_WRITE();

    /* Enable alarm interrupt */
    RTC->CRH |= RTC_CRH_ALRIE;

    RTC_WAIT_LAST_WRITE();
    
    /* Set RTC COUNTER to 0 */
    RTC->CNTH = 0 >> 16;
    RTC->CNTL = (0 & RTC_LSB_MASK);

    RTC_WAIT_LAST_WRITE();

    /* Exit config mode */
    RTC->CRL &= ~(uint16_t)RTC_CRL_CNF;

    RTC_WAIT_LAST_WRITE();

    /* Enable RTC alarm interrupt on EXTI */
    EXTI->IMR |= EXTI_Line17;
    EXTI->RTSR |= EXTI_Line17;
    EXTI->PR |= EXTI_Line17;

    /* Enable RTC alarm interrupt on NVIC, prio 3 */
    NVIC->ISER[((uint32_t)(RTCAlarm_IRQn) >> 5)] = (1 << ((uint32_t)(RTCAlarm_IRQn) & 0x1F));
    NVIC->IP[(uint32_t)(RTCAlarm_IRQn)] = ((3 << (8 - __NVIC_PRIO_BITS)) & 0xff);

    /* Enable RTC global interrupt on NVIC, prio 4 */
    NVIC->ISER[((uint32_t)(RTC_IRQn) >> 5)] = (1 << ((uint32_t)(RTC_IRQn) & 0x1F));
    NVIC->IP[(uint32_t)(RTC_IRQn)] = ((4 << (8 - __NVIC_PRIO_BITS)) & 0xff);
}

void RTC_IRQHandler(void)
{
    /* Enter config mode */
    RTC->CRL |= RTC_CRL_CNF;

    /* Clear pending alarm flag */
    RTC->CRL &= (uint16_t)~RTC_FLAG_ALR;   
     
    /* Exit config mode */
    RTC->CRL &= ~(uint16_t)RTC_CRL_CNF;

    /* Clear pending */
    EXTI->PR |= EXTI_Line17;
    NVIC->ICPR[((uint32_t)(RTC_IRQn) >> 5)] = (1 << ((uint32_t)(RTC_IRQn) & 0x1F));
    NVIC->ICPR[((uint32_t)(RTCAlarm_IRQn) >> 5)] = (1 << ((uint32_t)(RTCAlarm_IRQn) & 0x1F));
}

void RTCAlarm_IRQHandler(void)
{
    /* Enter config mode */
    RTC->CRL |= RTC_CRL_CNF;
    
    /* Clear pending alarm flag */
    RTC->CRL &= (uint16_t)~RTC_FLAG_ALR;

    /* Exit config mode */
    RTC->CRL &= ~(uint16_t)RTC_CRL_CNF;

    /* Clear pending */
    EXTI->PR |= EXTI_Line17;
    NVIC->ICPR[((uint32_t)(RTC_IRQn) >> 5)] = (1 << ((uint32_t)(RTC_IRQn) & 0x1F));
    NVIC->ICPR[((uint32_t)(RTCAlarm_IRQn) >> 5)] = (1 << ((uint32_t)(RTCAlarm_IRQn) & 0x1F));
}

void enter_low_power(uint32_t sleep_time)
{
    /* Enter config mode */
    RTC->CRL |= RTC_CRL_CNF;

    /* Get RTC COUNTER to 0 */
    uint32_t current_counter = ((RTC->CNTH) << 16) + (RTC->CNTL);
    
    /* Alarm is with certain period */
    current_counter += sleep_time;

    RTC_WAIT_LAST_WRITE();

    /* Set alarm count as certain period */
    RTC->ALRH = current_counter >> 16;
    RTC->ALRL = (current_counter & RTC_LSB_MASK);

    /* Exit config mode */
    RTC->CRL &= ~(uint16_t)RTC_CRL_CNF;

    /* Stop mode with sram reserved but pll and peripheral clock down. */
    PWR->CR |= PWR_CR_LPDS;

    /* Sleep */
    __ASM ("wfi");

    /* Switch to HSI as the default clock */
    RCC->CR |= RCC_CR_HSION;

    /* Wait till HSI is ready */
    while(!(RCC->CR & RCC_CR_HSIRDY))
    {}

    /* Select HSI as the system clock */
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_HSI;

    /* Make sure PLL is off */
    while ((RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS_PLL)
    {}

    /* Exit sleep and recover HSE */
    sys_clock_init();
}