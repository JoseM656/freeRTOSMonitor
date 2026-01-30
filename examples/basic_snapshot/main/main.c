#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "monitor.h"


/*Wait 2 seconds and call freeRTOSMonitor*/
void app_main(void)
{
    printf("\nBoot OK\n");

   
    vTaskDelay(pdMS_TO_TICKS(2000));

    
    freeRTOSMonitorSnapshotAll();

    
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}
