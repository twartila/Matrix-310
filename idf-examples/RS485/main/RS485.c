#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "driver/uart.h"
#include "freertos/queue.h"
#include "include/crc16.h"
#include "include/Artila-Matrix310.h"
/*
   This is a example which echos any data it receives on UART back to the sender using RS485 interface in half duplex mode.
*/
#define ECHO_TEST_TXD (COM1_TX)
#define ECHO_TEST_RXD (COM1_RX)

// RTS for RS485 Half-Duplex Mode manages DE/~RE
#define ECHO_TEST_RTS (COM1_RTS)

// CTS is not used in RS485 Half-Duplex Mode
#define ECHO_TEST_CTS (UART_PIN_NO_CHANGE)

#define BUF_SIZE (127)
#define BAUD_RATE (9600)

// Read packet timeout
#define PACKET_READ_TICS (100 / portTICK_RATE_MS)
#define ECHO_TASK_STACK_SIZE (2048)
#define ECHO_TASK_PRIO (10)
#define ECHO_UART_PORT 2 // You may use UART1 or UART2

// Timeout threshold for UART = number of symbols (~10 tics) with unchanged state on receive pin
#define ECHO_READ_TOUT (3) // 3.5T * 8 = 28 ticks, TOUT=3 -> ~24..33 ticks

static void echo_send(const int port, const char *str, uint8_t length)
{
    if (uart_write_bytes(port, str, length) != length)
    {
        printf("Send data critical failure.");
        // add your code to handle sending failure here
        abort();
    }
}

uint16_t swap_uint16(uint16_t val) {
    return (val << 8) | (val >> 8);
}

// An example of echo test with hardware flow control on UART
static void echo_task(void *arg)
{
    const int uart_num = ECHO_UART_PORT;
    uart_config_t uart_config = {
        .baud_rate = BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 122,
        .source_clk = UART_SCLK_APB,
    };

    printf("Start RS485 application test and configure UART.\n");

    // Install UART driver (we don't need an event queue here)
    // In this example we don't even use a buffer for sending data.
    ESP_ERROR_CHECK(uart_driver_install(uart_num, BUF_SIZE * 2, 0, 0, NULL, 0));

    // Configure UART parameters
    ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));

    printf("UART set pins, mode and install driver.\n");

    // Set UART pins
    ESP_ERROR_CHECK(uart_set_pin(uart_num, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS));

    // Set RS485 half duplex mode
    ESP_ERROR_CHECK(uart_set_mode(uart_num, UART_MODE_RS485_HALF_DUPLEX));

    // Set read timeout of UART TOUT feature
    ESP_ERROR_CHECK(uart_set_rx_timeout(uart_num, ECHO_READ_TOUT));

    // Allocate buffers for UART
    uint8_t *data = (uint8_t *)malloc(sizeof(uint8_t) * BUF_SIZE);
    // You may change the writeMsg according to your device setting.
    uint8_t writeModbus[] = {0x02, 0x03, 0x00, 0x44, 0x00, 0x03, 0x00, 0x00};
    // Generate CRC in Little Endian
    unsigned short crc = do_crc(&writeModbus[0], sizeof(writeModbus) - 2);
    printf("UART start recieve loop.\n");
    printf("Start RS485 UART test\n");
    // Print CRC in Big Endian
    printf("CRC: 0x%.2X\n", swap_uint16(crc));
    // Assign CRC to the last two bytes in Little Endian
    *(u_int16_t *)(&writeModbus[0] + (sizeof(writeModbus) - 2)) = crc;
    
    while (1)
    {
        // Write data to UART
        printf("Writed %u bytes:\n", sizeof(writeModbus));
        printf("[ ");
        for (int i = 0; i < sizeof(writeModbus); i++)
        {
            printf("0x%.2X ", (uint8_t)writeModbus[i]);
            echo_send(uart_num, (const char *)&writeModbus[i], 1);
        }
        printf("] \n");
        // Read data from UART
        int len = uart_read_bytes(uart_num, data, BUF_SIZE, PACKET_READ_TICS);
        if (len > 0)
        {
            printf("Received %u bytes\n:", len);
            printf("[ ");
            for (int i = 0; i < len; i++)
            {
                printf("0x%.2X ", (uint8_t)data[i]);
            }
            printf("] \n");
        }
        else
        {
            ESP_ERROR_CHECK(uart_wait_tx_done(uart_num, 10));
            printf("read nothing!\n");
        }
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void app_main(void)
{
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    printf("\nRS485 example:\n");
    // A uart read/write example without event queue;
    xTaskCreate(echo_task, "uart_echo_task", ECHO_TASK_STACK_SIZE, NULL, ECHO_TASK_PRIO, NULL);
}
