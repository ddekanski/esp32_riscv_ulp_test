# LED blink and ULP/deep sleep current consumption example using RISC-V ULP processor onboard ESP32-S3 with only C code (no assembler)
This code works with the LILYGO T7 S3 module based on ESP32-S3 (https://github.com/Xinyuan-LilyGO/T7-S3).

The S3 variant features a new RISC-V Ultra-Low-Power coprocessor that can be programmed using standard GNU tools included in ESP-IDF. This program has been built and flashed using VS Code with the ESP-IDF plugin installed (neither Arduino IDE nor Platformio support RISC-V ULP at the time of writing).

This code enables ULP and deep sleep current measurements as it cycles through the following phases:
1. Launch main CPU, initialize things, launch the ULP and go to deep sleep
2. Blink the on-board LED N times quickly (N relates to the ULP run count)
3. Keep the ULP busy with LED turned off for additional 2 secs
4. Every fourth ULP run, wake up the main CPU to print the current ULP run count and go to deep sleep again
4. Turn off the ULP for 3 secs until the timer wakes it up again
5. Go to (2)

Here are the current consumtion resuls for my LILYGO T7 S3:

| State      | Current consumption |
| ----------- | ----------- |
| Deep sleep, ULP not running phase 4) | 34µA       |
| Deep sleep, ULP running (phase 3)    | 416µA      |

The on-board LED in LILYGO T7 S3 is attached to GPIO 17. If you own a different module with different wiring, edit this line in ulp/main.c:
```
#define GPIO_LED GPIO_NUM_17
```
