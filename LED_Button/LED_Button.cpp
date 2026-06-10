#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/uart.h"


#define BAUD_RATE 115200
#define SWITCH_PIN 5
#define LED_PIN 17


int main()
{
    stdio_init_all();

    // Initialise the Wi-Fi chip
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
        return -1;
    }
    gpio_init(SWITCH_PIN);
    gpio_set_dir(SWITCH_PIN, GPIO_IN);

    // Enable internal pull-up resistor
    gpio_pull_up(SWITCH_PIN);
    
    // Example to turn on the Pico W LED
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, true);
    gpio_put(LED_PIN, true);
    bool last_state = gpio_get(SWITCH_PIN);

    while (true) {
    bool current_state = gpio_get(SWITCH_PIN);

    if (current_state != last_state) {
        if (!current_state) {
            printf("Switch ON\n");
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, true);
            gpio_put(LED_PIN, true);
        } else {
            printf("Switch OFF\n");
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, false);
                gpio_put(LED_PIN, false);

        }

        last_state = current_state;
        sleep_ms(200); // Debounce delay
    }
}
}