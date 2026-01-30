#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "monitor.h"


/*Wait 2 seconds and call individual freeRTOSMonitor. You can call any function you want.*/
void app_main(void)
{
    printf("\nBoot OK\n");

   
    vTaskDelay(pdMS_TO_TICKS(2000));

    
    printf("Just take the uptime:\n");
    freeRTOSMonitorSnapshotUptime();

    printf("Just take the heap:\n");
    freeRTOSMonitorSnapshotHeap();

    
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
