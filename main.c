#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "sub/test.h"

void RGB_task(){
    RP2040_zero_test_code();
}

int main()
{
    stdio_init_all();
    while(1){
        printf("hi\n");
        sleep_ms(100);
    }
    // xTaskCreate(RGB_task,"rgb_task",256,NULL,1,NULL);
    // vTaskStartScheduler();
    // printf("hello");
}