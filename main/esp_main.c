
#include <stdio.h>
#include "esp_sleep.h"
#include "ulp_riscv.h"
#include "ulp_main.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "ulp_main.h"

extern const uint8_t ulp_main_bin_start[] asm("_binary_ulp_main_bin_start");
extern const uint8_t ulp_main_bin_end[] asm("_binary_ulp_main_bin_end");

void delay_millis(int millis)
{
    vTaskDelay(millis / portTICK_PERIOD_MS);
}

bool is_ulp_inited() {
    return ulp_is_inited == 1234567890;
}

void app_main(void)
{
    // you need to enable main CPU wake up from ULP each time the main CPU is restarted (wake-up restarts the main CPU)
    ESP_ERROR_CHECK(esp_sleep_enable_ulp_wakeup());

    // this delay is required to properly launch the ULP program and put the main CPU to deep sleep
    // when there's native USB communication involved
    delay_millis(1000);

    printf("Wake-up cause = %i \n", esp_sleep_get_wakeup_cause());

    // For some reason, esp_sleep_get_wakeup_cause() doesn't work properly on ESP32-S3  (it always returns ESP_SLEEP_WAKEUP_UNDEFINED),
    // we need to have a variable that gets set to a magic number when the ULP program gets executed
    // so that we know here that is the variable is equal to that magic number, the ULP program has been launched
    // which means we have already properly loaded and initiated it.
    if (!is_ulp_inited())
    {
        printf("Not a ULP-RISC-V wakeup, loading the ULP program \n");
        ESP_ERROR_CHECK(ulp_riscv_load_binary(ulp_main_bin_start, (ulp_main_bin_end - ulp_main_bin_start)));
        ulp_measurement_count = 0;
        ESP_ERROR_CHECK(esp_sleep_enable_ulp_wakeup());
    }
    else
    {
        printf("ULP-RISC-V woke up the main CPU, measurement_count = %i\n", (int)ulp_measurement_count);
    }

    printf("Entering deep sleep...\n\n");

    // small delay to ensure the messages are printed
    delay_millis(100);

    if (!is_ulp_inited()) {
        // set ULP wake-up delay in us after each time after its main() returns
        ulp_set_wakeup_period(0, 3000 * 1000);
        // launch the ULP - the ULP program will get executed immediately, 
        // w/out waiting for the main CPU to enter deep sleep or the wakeup period to elapse
        // (there's no way to postpone its first execution)
        ESP_ERROR_CHECK(ulp_riscv_run());
    }

    // switch the main CPU to the deep sleep mode; it will be woken up by the ULP using ulp_riscv_wakeup_main_processor()
    esp_deep_sleep_start();
}

