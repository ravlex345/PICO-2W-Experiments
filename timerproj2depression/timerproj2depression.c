#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
static bool led_state = false;

void led_init(){
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed\n");
    }
}
bool toggle_led(struct repeating_timer *rt)
{
    led_state = !led_state;
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_state);
    return true; // Return true to keep the timer running
}

void timer_init(){
    struct repeating_timer timer;
    add_repeating_timer_ms(1000, toggle_led, NULL, &timer);
}

int main()
{
    stdio_init_all();

    led_init();
    
    while (true) {
        printf("Hello, world!\n");
        timer_init();
        

    }
}
