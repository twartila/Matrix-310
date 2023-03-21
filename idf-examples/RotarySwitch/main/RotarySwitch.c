/*
Rotary Switch Truth Table:
number  SW-4 SW-2 SW-1
0       1    1    1
1       1    1    0
2       1    0    1
3       1    0    0
4       0    1    1
5       0    1    0
6       0    0    1
7       0    0    0
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <driver/gpio.h>
#include "include/Artila-Matrix310.h"

int readRotarySwitch(){
    int total=0;
    if (gpio_get_level(SW_1) == 0) { total+=1; }
    if (gpio_get_level(SW_2) == 0) { total+=2; }
    if (gpio_get_level(SW_4) == 0) { total+=4; }
    return total;
}

static void configure_rotary_switch(void){
    printf("configured rotary switch!\n");
    gpio_reset_pin(SW_1); // SW_1: 34pin
    /* Set the GPIO as a push/pull input */
    gpio_set_direction(SW_1, GPIO_MODE_INPUT);
    gpio_reset_pin(SW_2); //SW_2: 26pin
    /* Set the GPIO as a push/pull input */
    gpio_set_direction(SW_2, GPIO_MODE_INPUT);
    gpio_reset_pin(SW_4); //SW_4: 35pin
    /* Set the GPIO as a push/pull input */
    gpio_set_direction(SW_4, GPIO_MODE_INPUT);
}



void app_main(void)
{
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    printf("\nRotary switch example:\n");
    /* Configure DIO */
    configure_rotary_switch();
    printf("Rotary switch value is: %d\n", readRotarySwitch());
}

