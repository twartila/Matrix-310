/*
    GPIO & RTC GPIO: 
    https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/gpio.html#_CPPv416gpio_config_t
*/ 
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <driver/gpio.h>
#include "include/Artila-Matrix310.h"

#define DI_PIN_SEL ((1ULL<<DI1) | (1ULL<<DI2))
#define DO_PIN_SEL (1ULL<<DO1)
#define DELAY_TIME 2000
static uint8_t state = 0;
/*
DO1: pin25
DI1: pin36
DI2: pin39
*/
static void setDO(void)
{
    /* Set the GPIO level according to the state (LOW or HIGH)*/
    gpio_set_level(DO1, state);
}

static void configure_dio(void)
{
    printf("Configured DIO!\n");
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = DO_PIN_SEL;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //enable pull-up mode
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);

    io_conf.intr_type = GPIO_INTR_ANYEDGE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = DI_PIN_SEL;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //enable pull-up mode
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);
}

static void configure_dio_simply(void){
    gpio_reset_pin(DO1);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(DO1, GPIO_MODE_OUTPUT);
    gpio_reset_pin(DI1);
    /* Set the GPIO as a push/pull input */
    gpio_set_direction(DI1, GPIO_MODE_INPUT);
    gpio_reset_pin(DI2);
    /* Set the GPIO as a push/pull input */
    gpio_set_direction(DI2, GPIO_MODE_INPUT);
}

void app_main(void)
{
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    printf("\nDIO example:\n");
    static uint8_t DI1_state = 0;
    static uint8_t DI2_state = 0;
    /* Configure DI/DO
    Both setting methods can work*/
    configure_dio(); // detailed settings
    // configure_dio_simply(); // simple setting
    while (1) {
        printf("Set DO1 %s!\n", state == 0 ? "HIGH" : "LOW");
        setDO();
        /* Toggle the DO1 state */
        state = !state;
        DI1_state = gpio_get_level(DI1);
        DI2_state = gpio_get_level(DI2);
        
        printf("DI1: %d\nDI2: %d\n", DI1_state, DI2_state);
        vTaskDelay(DELAY_TIME / portTICK_PERIOD_MS);
    }
}

