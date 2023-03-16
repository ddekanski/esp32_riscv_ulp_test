/* ULP-RISC-V example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.

   This code runs on ULP-RISC-V  coprocessor
*/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "ulp_riscv.h"
#include "ulp_riscv_utils.h"
#include "ulp_riscv_gpio.h"
#include "soc/rtc_cntl_reg.h"

#define GPIO_LED GPIO_NUM_17

volatile int measurement_count;
volatile int is_inited;

void delay_millis(int millis)
{
    ulp_riscv_delay_cycles(ULP_RISCV_CYCLES_PER_MS * millis);
}

void init_led()
{
    ulp_riscv_gpio_init(GPIO_LED);
    ulp_riscv_gpio_output_enable(GPIO_LED);
    ulp_riscv_gpio_set_output_mode(GPIO_LED, RTCIO_MODE_OUTPUT);
}

void set_led(bool state)
{
    ulp_riscv_gpio_output_level(GPIO_LED, state);
}

void count_led(int count) {
    while(count != 0) {
        set_led(true);
        delay_millis(50);
        set_led(false);
        delay_millis(100);
        count--;
    }
    delay_millis(500);
}

void mark_ulp_as_inited() {
    is_inited = 1234567890;
}

int main(void)
{
    mark_ulp_as_inited();

    init_led();

    measurement_count++;

    // blink the LED as many times as measurement_count
    count_led(measurement_count);

    // do nothing for a while to allow measuring real ULP current consumption
    delay_millis(2000);

    // wake up the main CPU on every fourth measurement
    if (measurement_count % 4 == 0) {
        ulp_riscv_wakeup_main_processor();
    }

    // stop th ULP until it's woken up again after 3s as defined in esp_main.c using ulp_set_wakeup_period()
    // to measure pure deep-sleep current consumption between ULP wake-ups
    return 0;
}