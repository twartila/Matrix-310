#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_netif.h"
#include "esp_eth.h"
#include "esp_event.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "lwip/ip_addr.h"
#include "driver/spi_master.h"
#include "include/Artila-Matrix310.h"

#define CONFIG_EXAMPLE_USE_SPI_ETHERNET 1
#define CONFIG_EXAMPLE_USE_W5500 1
#define CONFIG_EXAMPLE_ETH_SPI_CLOCK_MHZ 12
#define CONFIG_EXAMPLE_ETH_SPI_HOST 1

/* Event handler for Ethernet events */
static void eth_event_handler(void *arg, esp_event_base_t event_base,
                              int32_t event_id, void *event_data)
{
    uint8_t mac_addr[6] = {0};
    /* We can get the ethernet driver handle from event data */
    esp_eth_handle_t eth_handle = *(esp_eth_handle_t *)event_data;

    switch (event_id)
    {
    case ETHERNET_EVENT_CONNECTED:
        esp_eth_ioctl(eth_handle, ETH_CMD_G_MAC_ADDR, mac_addr);
        printf("Ethernet Link Up\n");
        printf("Ethernet HW Addr %02x:%02x:%02x:%02x:%02x:%02x\n",
                 mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
        break;
    case ETHERNET_EVENT_DISCONNECTED:
        printf("Ethernet Link Down\n");
        break;
    case ETHERNET_EVENT_START:
        printf("Ethernet Started\n");
        break;
    case ETHERNET_EVENT_STOP:
        printf("Ethernet Stopped\n");
        break;
    default:
        break;
    }
}

/* Event handler for IP_EVENT_ETH_GOT_IP */
static void got_ip_event_handler(void *arg, esp_event_base_t event_base,
                                 int32_t event_id, void *event_data)
{
    ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
    const esp_netif_ip_info_t *ip_info = &event->ip_info;

    printf("Ethernet Got DHCP IP Address\n");
    printf("~~~~~~~~~~~\n");
    printf("ETHIP:" IPSTR"\n", IP2STR(&ip_info->ip));
    printf("ETHMASK:" IPSTR"\n", IP2STR(&ip_info->netmask));
    printf("ETHGW:" IPSTR"\n", IP2STR(&ip_info->gw));
    printf("~~~~~~~~~~~\n");
}

void app_main(void)
{
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    printf("\nEthernet DHCP example:\n");
    // Initialize TCP/IP network interface (should be called only once in application)
    ESP_ERROR_CHECK(esp_netif_init());
    // Create default event loop that running in background
    ESP_ERROR_CHECK(esp_event_loop_create_default());
#if CONFIG_EXAMPLE_USE_SPI_ETHERNET
    // Create instance(s) of esp-netif for SPI Ethernet(s)
    esp_netif_inherent_config_t esp_netif_config = ESP_NETIF_INHERENT_DEFAULT_ETH();
    esp_netif_config_t cfg_spi = {
        .base = &esp_netif_config,
        .stack = ESP_NETIF_NETSTACK_DEFAULT_ETH};
    esp_netif_t *eth_netif_spi = {NULL};
    char if_key_str[10];
    char if_desc_str[10];
    char num_str[3];

    itoa(0, num_str, 10);
    strcat(strcpy(if_key_str, "ETH_SPI_"), num_str);
    strcat(strcpy(if_desc_str, "eth"), num_str);
    esp_netif_config.if_key = if_key_str;
    esp_netif_config.if_desc = if_desc_str;
    esp_netif_config.route_prio = 30 - 0;
    eth_netif_spi = esp_netif_new(&cfg_spi);

    // Init MAC and PHY configs to default
    eth_mac_config_t mac_config_spi = ETH_MAC_DEFAULT_CONFIG();
    eth_phy_config_t phy_config_spi = ETH_PHY_DEFAULT_CONFIG();

    // Install GPIO ISR handler to be able to service SPI Eth modlues interrupts
    gpio_install_isr_service(0);

    // Init SPI bus
    spi_device_handle_t spi_handle = {NULL};
    spi_bus_config_t buscfg = {
        .miso_io_num = LAN_MISO,
        .mosi_io_num = LAN_MOSI,
        .sclk_io_num = LAN_SCLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
    };
    ESP_ERROR_CHECK(spi_bus_initialize(CONFIG_EXAMPLE_ETH_SPI_HOST, &buscfg, SPI_DMA_CH_AUTO));


    // Configure SPI interface and Ethernet driver for specific SPI module
    esp_eth_mac_t *mac_spi = malloc(sizeof(esp_eth_mac_t));
    esp_eth_phy_t *phy_spi = malloc(sizeof(esp_eth_phy_t));
    esp_eth_handle_t eth_handle_spi = {NULL};

#if CONFIG_EXAMPLE_USE_W5500
    // This is a configuration for a SPI slave device that is connected to one of the SPI buses.
    spi_device_interface_config_t devcfg = {
        .command_bits = 16,
        .address_bits = 8,
        .mode = 0,
        .clock_speed_hz = CONFIG_EXAMPLE_ETH_SPI_CLOCK_MHZ * 1000 * 1000,
        .queue_size = 20};

    // Set SPI module Chip Select GPIO
    devcfg.spics_io_num = LAN_CS;

    ESP_ERROR_CHECK(spi_bus_add_device(CONFIG_EXAMPLE_ETH_SPI_HOST, &devcfg, &spi_handle));
    // w5500 ethernet driver is based on spi driver
    eth_w5500_config_t w5500_config = ETH_W5500_DEFAULT_CONFIG(spi_handle);

    // Set remaining GPIO numbers and configuration used by the SPI module
    phy_config_spi.phy_addr = LAN_PHY_ADDR;
    phy_config_spi.reset_gpio_num = LAN_PHY_RST;

    mac_spi = esp_eth_mac_new_w5500(&w5500_config, &mac_config_spi);
    phy_spi = esp_eth_phy_new_w5500(&phy_config_spi);
#endif // CONFIG_EXAMPLE_USE_W5500

    esp_eth_config_t eth_config_spi = ETH_DEFAULT_CONFIG(mac_spi, phy_spi);
    ESP_ERROR_CHECK(esp_eth_driver_install(&eth_config_spi, &eth_handle_spi));

    /* The SPI Ethernet module might not have a burned factory MAC address, we cat to set it manually.
       02:00:00 is a Locally Administered OUI range so should not be used except when testing on a LAN under your control.
    */
    ESP_ERROR_CHECK(esp_eth_ioctl(eth_handle_spi, ETH_CMD_S_MAC_ADDR, (uint8_t[]){0x02, 0x00, 0x00, 0x12, 0x34, 0x56 + CONFIG_EXAMPLE_ETH_SPI_HOST - 1}));

    // Attach Ethernet driver to TCP/IP stack
    ESP_ERROR_CHECK(esp_netif_attach(eth_netif_spi, esp_eth_new_netif_glue(eth_handle_spi)));
#endif // CONFIG_ETH_USE_SPI_ETHERNET

    // Register user defined event handers
    ESP_ERROR_CHECK(esp_event_handler_register(ETH_EVENT, ESP_EVENT_ANY_ID, &eth_event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_ETH_GOT_IP, &got_ip_event_handler, NULL));

    /* Start Ethernet driver state machine */
#if CONFIG_EXAMPLE_USE_SPI_ETHERNET

    ESP_ERROR_CHECK(esp_eth_start(eth_handle_spi));

#endif // CONFIG_EXAMPLE_USE_SPI_ETHERNET
}