# LED blink example using RISC-V ULP processor onboard ESP32-S3 with only C code (no assembler)
This code works with the LILYGO T7 S3 module based on ESP32-S3 (https://github.com/Xinyuan-LilyGO/T7-S3).

The S3 variant features a new RISC-V Ultra-Low-Power coprocessor that can be programmed using standard GNU tools included in ESP-IDF.

For some reason, the ULP code does not launch when the module is USB-connected to the PC. After building and flashing the code, disconnect the module from the PC, attach a LiPo battery and press the RST button on the module - the on-board LED will start blinking.

The on-board LED in LILYGO T7 S3 is attached to GPIO 17. If you own a different module with different wiring, edit this line in ulp/main.c:
```
#define GPIO_LED GPIO_NUM_17
```
