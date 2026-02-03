/*
 * José "Foten" Morata (josemanuelfabris@gmail.com)
 *
 * To run this code, you need the ESP-IDF toolset.
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_heap_caps.h"

#include "monitor.h"


/*FORWARD DECLARATIONS (INTERNAL)*/
static void freeRTOSMonitorSnapshotALLInternal(void);
static void freeRTOSMonitorSnapshotUptimeInternal(void);
static void freeRTOSMonitorSnapshotHeapInternal(void);
static void freeRTOSMonitorSnapshotSchedulerInternal(void);
static void freeRTOSMonitorSnapshotCpuInternal(void);



/* PUBLIC API (wrappers)*/

/*A function that takes a snapshot of ALL, scheduler, 
RAM, uptime, and CPU and prints it via UART.*/
void freeRTOSMonitorSnapshotAll(void)
{ 
    freeRTOSMonitorSnapshotALLInternal();
}

/*Small function that takes a snapshot of Uptime.*/
void freeRTOSMonitorSnapshotUptime(void)
{
    freeRTOSMonitorSnapshotUptimeInternal();
}

/*Small function that takes a snapshot of Heap.*/
void freeRTOSMonitorSnapshotHeap(void)
{
    freeRTOSMonitorSnapshotHeapInternal();
}


/*A small function that takes a snapshot of the scheduler.*/
void freeRTOSMonitorSnapshotScheduler(void)
{
    freeRTOSMonitorSnapshotSchedulerInternal();
}

/*Use system functions to calculate CPU usage.*/
void freeRTOSMonitorSnapshotCpu(void)
{
    freeRTOSMonitorSnapshotCpuInternal();
}


/*A function that takes a snapshot of ALL, scheduler, 
RAM, uptime, and CPU and prints it via UART. You can use it in a missed deadline.*/
static void freeRTOSMonitorSnapshotALLInternal(void) {

    printf("----- [freeRTOSMonitor] - GENERAL SNAPSHOT -----\n");

    freeRTOSMonitorSnapshotUptimeInternal();
    freeRTOSMonitorSnapshotHeapInternal();
    freeRTOSMonitorSnapshotSchedulerInternal();
    freeRTOSMonitorSnapshotCpuInternal();
 
    
    printf("============================\n");

}

/*Small function that takes a snapshot of Uptime*/
static void freeRTOSMonitorSnapshotUptimeInternal(void) {

    uint32_t uptime_ms = xTaskGetTickCount() * portTICK_PERIOD_MS;
    printf("[freeRTOSMonitor] Uptime: %lu ms (%lu s)\n", uptime_ms, uptime_ms / 1000);

}

/*Small function that takes a snapshot of Heap*/
static void freeRTOSMonitorSnapshotHeapInternal(void) {

    size_t totalHeap = heap_caps_get_total_size(MALLOC_CAP_8BIT);
    size_t freeHeap  = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    size_t usedPercent = ((totalHeap - freeHeap) * 100 ) / totalHeap;

    printf("[freeRTOSMonitor] Heap total: %zu bytes - Free heap: %zu bytes\n", totalHeap, freeHeap);
    printf("[freeRTOSMonitor] Heap used: %zu (%zu%%) bytes\n", totalHeap - freeHeap, usedPercent);

}


/*A small function that takes a snapshot of the scheduler.*/
static void freeRTOSMonitorSnapshotSchedulerInternal(void) {

    /*Adjust the buffer according to the number of tasks*/
    size_t buf_size = uxTaskGetNumberOfTasks() * 80;
    char *buf = (char *)pvPortMalloc(buf_size);
    
     if (buf == NULL) {
        printf("[freeRTOSMonitor] ERROR: no heap\n");
        return;
    }

    printf("\n----- [freeRTOSMonitor] - SCHEDULER SNAPSHOT -----\n");
    printf("Task\t\tState\tPrio\tStack\tNum\n");

    vTaskList(buf);
    printf("%s\n", buf);


    vPortFree(buf);
}


/*Use system functions to calculate CPU usage*/
static void freeRTOSMonitorSnapshotCpuInternal(void) {

    UBaseType_t task_count = uxTaskGetNumberOfTasks();
    if (task_count == 0) return;

    TaskStatus_t *status =
        pvPortMalloc(task_count * sizeof(TaskStatus_t));

    if (!status) {
        printf("[freeRTOSMonitor] No memory!\n");
        return;
    }

    uint32_t total_runtime = 0;
    task_count = uxTaskGetSystemState(status, task_count, &total_runtime);

    if (total_runtime == 0) {
        vPortFree(status);
        return;
    }

    printf("\n----- [freeRTOSMonitor] - CPU SNAPSHOT -----\n");
    printf("Task\t\tCPU %%\tRuntime\n");
    for (UBaseType_t i = 0; i < task_count; i++) {
        uint32_t cpu =
            (status[i].ulRunTimeCounter * 100) / total_runtime;

        printf("%s\t\t%lu%%\t%lu\n",
               status[i].pcTaskName,
               cpu,
               status[i].ulRunTimeCounter);
    }

    vPortFree(status);
}
