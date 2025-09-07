@ stm32l152 has 0x8000 (32K) SRAM
@ SRAM is starting from 0x20000000
@ so stack is eq 0x20000000 + 0x8000 = 0x20008000
@ flash starts from 0x08000000

.syntax unified
.thumb
.cpu cortex-m3

.equ Stack_up, 0x20008000

.section    .text.vector_table
.type   vector_table, %object

vector_table:
    .word  Stack_up
    .word  Reset_Handler
    .word  Default_Handler              @    .word  NMI_Handler
    .word  Default_Handler              @    .word  HardFault_Handler
    .word  Default_Handler              @    .word  MemManage_Handler
    .word  Default_Handler              @    .word  BusFault_Handler
    .word  Default_Handler              @    .word  UsageFault_Handler
    .word  0                            @    .word  0
    .word  0                            @    .word  0
    .word  0                            @    .word  0
    .word  0                            @    .word  0
    .word  0                            @    .word  SVC_Handler
    .word  0                            @    .word  DebugMon_Handler
    .word  0                            @    .word  0
    .word  0                            @    .word  PendSV_Handler
    .word  0                            @    .word  SysTick_Handler
    .word  0                            @    .word  WWDG_IRQHandler
    .word  0                            @    .word  PVD_IRQHandler
    .word  0                            @    .word  TAMPER_IRQHandler
    .word  0                            @    .word  RTC_IRQHandler
    .word  0                            @    .word  FLASH_IRQHandler
    .word  0                            @    .word  RCC_IRQHandler
    .word  0                            @    .word  EXTI0_IRQHandler
    .word  0                            @    .word  EXTI1_IRQHandler
    .word  0                            @    .word  EXTI2_IRQHandler
    .word  0                            @    .word  EXTI3_IRQHandler
    .word  0                            @    .word  EXTI4_IRQHandler
    .word  0                            @    .word  DMA1_Channel1_IRQHandler
    .word  0                            @    .word  DMA1_Channel2_IRQHandler
    .word  0                            @    .word  DMA1_Channel3_IRQHandler
    .word  0                            @    .word  DMA1_Channel4_IRQHandler
    .word  0                            @    .word  DMA1_Channel5_IRQHandler
    .word  0                            @    .word  DMA1_Channel6_IRQHandler
    .word  0                            @    .word  DMA1_Channel7_IRQHandler
    .word  0                            @    .word  ADC1_2_IRQHandler
    .word  0                            @    .word  USB_HP_CAN1_TX_IRQHandler
    .word  0                            @    .word  USB_LP_CAN1_RX0_IRQHandler
    .word  0                            @    .word  CAN1_RX1_IRQHandler
    .word  0                            @    .word  CAN1_SCE_IRQHandler
    .word  0                            @    .word  EXTI9_5_IRQHandler
    .word  0                            @    .word  TIM1_BRK_IRQHandler
    .word  0                            @    .word  TIM1_UP_IRQHandler
    .word  0                            @    .word  TIM1_TRG_COM_IRQHandler
    .word  0                            @    .word  TIM1_CC_IRQHandler
    .word  0                            @    .word  TIM2_IRQHandler
    .word  0                            @    .word  TIM3_IRQHandler
    .word  0                            @    .word  TIM4_IRQHandler
    .word  0                            @    .word  I2C1_EV_IRQHandler
    .word  0                            @    .word  I2C1_ER_IRQHandler
    .word  0                            @    .word  I2C2_EV_IRQHandler
    .word  0                            @    .word  I2C2_ER_IRQHandler
    .word  0                            @    .word  SPI1_IRQHandler
    .word  0                            @    .word  SPI2_IRQHandler
    .word  0                            @    .word  USART1_IRQHandler
    .word  0                            @    .word  USART2_IRQHandler
    .word  USART3_IRQHandler            @    .word  USART3_IRQHandler
    .word  0                            @    .word  EXTI15_10_IRQHandler
    .word  0                            @    .word  RTCAlarm_IRQHandler
    .word  0                            @    .word  USBWakeUp_IRQHandler  
    .word  0                            @    .word  0
    .word  0                            @    .word  0
    .word  0                            @    .word  0
    .word  0                            @    .word  0
    .word  0                            @    .word  0
    .word  0                            @    .word  0
    .word  0                            @    .word  0
    .word  0                            @    .word  BootRAM          /* @0x108. This is for boot in RAM mode for STM32F10x Medium Density devices. */

.size   vector_table, .-vector_table


.section    .text.crt0
.type   Reset_Handler, %function

Reset_Handler:
    bl  sys_clock_init
    b   main

.size   Reset_Handler, .-Reset_Handler


.section    .text.crt0
.type   Default_Handler, %function

Default_Handler:
    b   .

.size   Default_Handler, .-Default_Handler
