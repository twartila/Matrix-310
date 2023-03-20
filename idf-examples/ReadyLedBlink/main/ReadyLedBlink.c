/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <driver/gpio.h>
#include "include/Artila-Matrix310.h"

#define CONFIG_BLINK_PERIOD 2000
static const char *TAG = "ReadyLED blink example: ";

static uint8_t s_led_state = 0;

static void blink_led(void)
{
    /* Set the GPIO level according to the state (LOW or HIGH)*/
    gpio_set_level(LED_READY, s_led_state);
}

static void configure_led(void)
{
    printf("\n%sConfigured to blink GPIO readyLED!\n", TAG);
    //LED_READY: pin27
    gpio_reset_pin(LED_READY);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(LED_READY, GPIO_MODE_OUTPUT);
}

void app_main(void)
{

    /* Configure the peripheral according to the LED type */
    configure_led();
    while (1) {
        printf("%sTurning the LED %s!\n",TAG, s_led_state == true ? "ON" : "OFF");
        blink_led();
        /* Toggle the LED state */
        s_led_state = !s_led_state;
        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
    }
}
