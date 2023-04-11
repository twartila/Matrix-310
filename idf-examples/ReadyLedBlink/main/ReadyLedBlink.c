#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <driver/gpio.h>
#include "include/Artila-Matrix310.h"

#define CONFIG_BLINK_PERIOD 2000
static uint8_t s_led_state = 0;

static void blink_led(void)
{
    /* Set the GPIO level according to the state (LOW or HIGH)*/
    gpio_set_level(LED_READY, s_led_state);
}

static void configure_led(void)
{
    printf("Configured to blink GPIO readyLED!\n");
    //LED_READY: pin27
    gpio_reset_pin(LED_READY);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(LED_READY, GPIO_MODE_OUTPUT);
}

void app_main(void)
{
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    printf("\nReadyLED blink example:\n");
    /* Configure the peripheral according to the LED type */
    configure_led();
    while (1) {
        printf("Turning the LED %s!\n", s_led_state == true ? "ON" : "OFF");
        blink_led();
        /* Toggle the LED state */
        s_led_state = !s_led_state;
        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
    }
}
