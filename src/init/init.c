#include "uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "stm32f10x.h"
#include "delay.h"

char debug_char = 'H';
uint32_t task_flag[2] = {0xaa,0xaa};

SemaphoreHandle_t mutex;

void vApplicationIdleHook()
{
    __ASM ("wfi");
}

void test_a(void *p)
{
    while(1)
    {
        xSemaphoreTake(mutex, portMAX_DELAY);
        uart_send('a');
        xSemaphoreGive(mutex);
        vTaskDelay(100);
    }
}

void test_b(void *p)
{
    while(1)
    {
        xSemaphoreTake(mutex, portMAX_DELAY);
        uart_send('b');
        xSemaphoreGive(mutex);
        vTaskDelay(200);
    }
}


int main(void)
{
    uart_init();

    mutex = xSemaphoreCreateMutex();

    task_flag[0] = xTaskCreate(test_a, "test_a", 128, NULL, 4, NULL);
    task_flag[1] = xTaskCreate(test_b, "test_b", 128, NULL, 3, NULL);
    
    vTaskStartScheduler();

    while(1);
}
