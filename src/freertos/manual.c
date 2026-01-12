#include "FreeRTOS.h"
#include "task.h"

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    /* This function is called if a task overflows its stack. */
    /* Common practice is to disable interrupts and enter a loop. */
    taskDISABLE_INTERRUPTS();
    for( ;; );
}