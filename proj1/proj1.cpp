#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/clocks.h"
#include "hardware/pll.h"
#include "hardware/structs/pll.h"
#include "hardware/structs/clocks.h"



#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif 


int cust_gpio_init(){
    #if defined(CYW43_WL_GPIO_LED_PIN)
    // For Pico W devices we need to initialise the driver etc
    return cyw43_arch_init();
    #endif
}
void led_toggle(){
    #if defined(CYW43_WL_GPIO_LED_PIN)
        static bool led_on = false;
        led_on = !led_on;
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
    #endif
}



int main()
{
    stdio_init_all();
    cust_gpio_init();
    clock_configure(clk_sys,
        CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX,
        CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_PLL_USB,48 * MHZ, 48 * MHZ);
while (true) {
        printf("Nah\n");
        led_toggle();
        sleep_ms(1000);
    }
}
