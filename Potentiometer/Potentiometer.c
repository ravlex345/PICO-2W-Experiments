#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"

const uint8_t digits[10][5] = {
    {0x3E,0x51,0x49,0x45,0x3E}, // 0
    {0x00,0x42,0x7F,0x40,0x00}, // 1
    {0x42,0x61,0x51,0x49,0x46}, // 2
    {0x21,0x41,0x45,0x4B,0x31}, // 3
    {0x18,0x14,0x12,0x7F,0x10}, // 4
    {0x27,0x45,0x45,0x45,0x39}, // 5
    {0x3C,0x4A,0x49,0x49,0x30}, // 6
    {0x01,0x71,0x09,0x05,0x03}, // 7
    {0x36,0x49,0x49,0x49,0x36}, // 8
    {0x06,0x49,0x49,0x29,0x1E}  // 9
};

void oled_cmd(uint8_t cmd) {
    uint8_t buf[2] = {0x00, cmd};
    i2c_write_blocking(i2c0, 0x3C, buf, 2, false);
}

void oled_data(uint8_t* data, size_t len) {
    uint8_t buf[len + 1];
    buf[0] = 0x40;
    for (int i = 0; i < len; i++) buf[i + 1] = data[i];
    i2c_write_blocking(i2c0, 0x3C, buf, len + 1, false);
}
void oled_init() {
    sleep_ms(100);

    oled_cmd(0xAE); // display off
    oled_cmd(0x20); oled_cmd(0x00); // horizontal addressing
    oled_cmd(0xB0);
    oled_cmd(0xC8);
    oled_cmd(0x00);
    oled_cmd(0x10);
    oled_cmd(0x40);
    oled_cmd(0x81); oled_cmd(0x7F);
    oled_cmd(0xA1);
    oled_cmd(0xA6);
    oled_cmd(0xA8); oled_cmd(0x3F);
    oled_cmd(0xA4);
    oled_cmd(0xD3); oled_cmd(0x00);
    oled_cmd(0xD5); oled_cmd(0x80);
    oled_cmd(0xD9); oled_cmd(0xF1);
    oled_cmd(0xDA); oled_cmd(0x12);
    oled_cmd(0xDB); oled_cmd(0x40);
    oled_cmd(0x8D); oled_cmd(0x14);

    oled_cmd(0xAF); // display ON
}


void oled_clear() {
    for (int page = 0; page < 8; page++) {
        oled_cmd(0xB0 + page);
        oled_cmd(0x00);
        oled_cmd(0x10);

        uint8_t data[128];
        for (int i = 0; i < 128; i++) {
            data[i] = 0x00; // all pixels OFF
        }

        oled_data(data, 128);
    }
}
void oled_draw_digit(int column, char digit) {
    if (digit < '0' || digit > '9') return;

    int d = digit - '0';

    oled_cmd(0xB0);                 // page 0
    oled_cmd(column & 0x0F);
    oled_cmd(0x10 | (column >> 4));

    uint8_t buffer[6];

    for(int i = 0; i < 5; i++) {
        buffer[i] = digits[d][i];
    }

    buffer[5] = 0x00; // spacing

    oled_data(buffer, 6);
}

void oled_print_number(uint16_t value) {
    char str[8];

    sprintf(str, "%u", value);

    int col = 0;

    for(int i = 0; str[i] != '\0'; i++) {
        oled_draw_digit(col, str[i]);
        col += 6;
    }
}

int main() {
    stdio_init_all();

    adc_init();
    adc_gpio_init(27);   // GP26 = ADC0
    adc_select_input(1);

    sleep_ms(2000);
    i2c_init(i2c0, 100 * 1000);

    gpio_set_function(4, GPIO_FUNC_I2C);
    gpio_set_function(5, GPIO_FUNC_I2C);

    gpio_pull_up(4);
    gpio_pull_up(5);

    sleep_ms(500);

    oled_init();
    oled_clear();

    while (true) {
        oled_clear();

        uint16_t raw = adc_read();

        printf("ADC: %u\n", raw);
        oled_print_number(raw);
        sleep_ms(200);
    }
}