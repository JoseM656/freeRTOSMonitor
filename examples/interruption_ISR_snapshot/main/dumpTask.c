/*
 * José "Foten" Morata (josemanuelfabris@gmail.com)
 *
 * This is example code of how to use freeRTOSMonitor
 */

 
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "monitor.h"


#define BUTTON_GPIO 0   // GPIO 0 (BOOT on many ESP32)


struct system_status_t
{
    uint8_t cpu_percentage;
    uint8_t ram_percentage;
    uint8_t system_tem;
    uint8_t id_core;
    uint16_t uptime_ms;
};


SemaphoreHandle_t button_sem; // Create a semaphore to detect the button
QueueHandle_t data_queue; // global variable


// Start the ISR in RAM
void IRAM_ATTR button_isr(void *arg)
{
    BaseType_t task_woken = pdFALSE;
    xSemaphoreGiveFromISR(button_sem, &task_woken);
    if (task_woken) {
        portYIELD_FROM_ISR();
    }
}


// Function that waits in a blocking manner to receive a signal
void button_task(void *pvParameters)
{
    while (1) {
        if (xSemaphoreTake(button_sem, portMAX_DELAY)) {
            freeRTOSMonitorSnapshotAll();
        }
    }
}


// Configure and expose the GPIO
void configure_button(void)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << BUTTON_GPIO),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_NEGEDGE  // Falling edge
    };

    gpio_config(&io_conf);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUTTON_GPIO, button_isr, NULL);
}



// Core 0: Producer task
void producer_task(void *pvParameters){

    struct system_status_t data; // Instantiate the structure with a variable

    while (1){

        printf("[Core 0] Processing data...\n");
        // Every 1000ms the task wakes up and generates 3 pseudo-random data
        uint32_t t = xTaskGetTickCount();
        uint8_t cpu = t % 101;
        uint8_t ram = t % 101;
        uint8_t tem = t % 101;

        // Fill the structure with that data
        data.cpu_percentage = cpu;
        data.ram_percentage = ram;
        data.system_tem = tem;
        data.id_core = 0;
        data.uptime_ms = 1000;
        
        printf("[Core 0] Sending data...\n");
        xQueueSend(data_queue, &data, portMAX_DELAY); // Put the data instances into the queue
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
    
}


// Core 1: Consumer task
void consumer_task(void *pvParameters){

    struct system_status_t received; // receive the struct instances

    while (1)
    {
        if (xQueueReceive(data_queue, &received, portMAX_DELAY)) {
            printf("[Core 1] Data received from %d\n", received.id_core);
            printf("[Core 1] Wait time between cycles %d\n", received.uptime_ms);
            printf("[Core 1] CPU usage: %d\n", received.cpu_percentage);
            printf("[Core 1] RAM usage: %d\n", received.ram_percentage);
            printf("[Core 1] System temperature: %d\n", received.system_tem);
        }
    }
    
}


// Start a binary semaphore and a task
void app_main(void) {

    button_sem = xSemaphoreCreateBinary();
    if (button_sem == NULL) {
        printf("Error creating semaphore\n");
        return;
    }


    configure_button();

    data_queue = xQueueCreate(5, sizeof(struct system_status_t)); // how much is expected to be received from the queue

    /*Queue for producer consumer*/
    if (data_queue == NULL) {
        printf("[System] Error creating queue or queue empty");
        return;
    }


    /*Button task*/
    xTaskCreate(
        button_task,
        "button_task",
        2048,
        NULL,
        5,
        NULL
        
    );


    /*Create a simple producer consumer task*/
    xTaskCreate(producer_task, "Producer", 2048, NULL, 5, NULL);
    xTaskCreate(consumer_task, "Consumer", 2048, NULL, 5, NULL);
}