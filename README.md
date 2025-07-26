# RK3568 ‑WF EVB Kit & FCS866R Board Definitions

This repository provides **platformio‑style board definitions** for a smart‑display platform that
uses the **Rockchip RK3568 application processor** and the **Quectel FCS866R Wi‑Fi 6/Bluetooth 5.2
module**.  It is based on the structure of the [Sunton CYD board definitions project]
(https://github.com/rzeldent/platformio-espressif32-sunton), but instead of defining ESP32‑based
microcontroller boards it defines a host‑processor platform with an external wireless module.

The RK3568‑WF EVB kit is an evaluation board that pairs a **quad‑core Cortex‑A55 RK3568 SoC**
with a socket for Quectel’s FCS866R Wi‑Fi module.  The FCS866R provides dual‑band Wi‑Fi 6 and
Bluetooth 5.2 via an SDIO 3.0 interface and is mounted on the EVB’s M.2 connector.  Since
the RK3568 runs Linux, the application firmware and LVGL user interface must be built
for a Linux environment rather than the Arduino/ESP‑IDF used by ESP32 boards.  See the
`report.md` in the original analysis for more information about the redesign.

## Supported boards

The following boards mirror the **Sunton CYD smart‑display line** but are adapted for
the RK3568 application processor and the FCS866R wireless module.  Each JSON file
defines the display resolution, controller type, bus interface and optional
peripherals (touch controller, TF card, RGB LED, CDS light sensor and speaker).

| Board                                  | Display (size/resolution) | Display controller / bus  | Touch interface       | Extras (TF/LED/CDS/Speaker) |
| -------------------------------------- | ------------------------- | ------------------------- | --------------------- | -------------------------------- |
| `rk3568_1732S019C`                     | 1.9″ 170×320              | ST7789 via SPI           | GT911 (I2C)          | —                                |
| `rk3568_2424S012C`                     | 1.2″ round 240×240        | GC9A01A via SPI          | CST816S (I2C)        | —                                |
| `rk3568_2432S024C`                     | 2.4″ 240×320              | ILI9341 via SPI          | CST816S (I2C)        | TF card + RGB LED + CDS + speaker |
| `rk3568_2432S024R`                     | 2.4″ 240×320              | ILI9341 via SPI          | XPT2046 (SPI)        | TF card + RGB LED + CDS + speaker |
| `rk3568_2432S028R`                     | 2.8″ 240×320              | ILI9341 via SPI          | XPT2046 (SPI)        | TF card + RGB LED + CDS + speaker |
| `rk3568_4827S043C`                     | 4.3″ 480×272              | ST7262 16‑bit parallel   | GT911 (I2C)          | —                                |
| `rk3568_4827S043R`                     | 4.3″ 480×272              | ST7262 16‑bit parallel   | XPT2046 (SPI)        | TF card                           |
| `rk3568_8048S050C`                     | 5.0″ 800×480              | ST7262 16‑bit parallel   | GT911 (I2C)          | —                                |
| `rk3568_8048S050R`                     | 5.0″ 800×480              | ST7262 16‑bit parallel   | XPT2046 (SPI)        | TF card                           |
| `rk3568_8048S070C`                     | 7.0″ 800×480              | ST7262 16‑bit parallel   | GT911 (I2C)          | —                                |
| `rk3568_8048S070R`                     | 7.0″ 800×480              | ST7262 16‑bit parallel   | XPT2046 (SPI)        | TF card                           |

All boards assume **Wi‑Fi 6/Bluetooth 5.2 connectivity** through the Quectel
FCS866R module attached to the RK3568’s SDIO host.  Boards marked with `C` use a
capacitive touch controller (either GT911 or CST816S), while those marked with `R`
use the XPT2046 resistive controller.  Variants with a trailing `C`/`R` suffix are
similar to the original ESP32 boards but map their peripherals to the RK3568’s
interfaces and supply drivers accordingly.

## How to use these definitions

These JSON files are provided as a reference for integrating LVGL applications on the RK3568
platform.  They include compile‑time flags for display resolution and buffer sizes, and
indicate that the board supports Wi‑Fi 6 and Bluetooth 5.2 via SDIO and PCM/UART.  The
definitions are **not directly supported by PlatformIO**, because PlatformIO does not
currently support Linux hosts.  Instead, you should use these files as documentation for
configuring your build system (e.g. Yocto or Buildroot) and LVGL application on the RK3568.

## Files in this repository

The `boards/` directory now contains JSON definitions for all of the boards
listed above.  For example:

* `boards/rk3568_1732S019C.json` – 1.9″ 170×320 ST7789 display with GT911 touch.
* `boards/rk3568_2432S024C.json` – 2.4″ 240×320 ILI9341 panel with capacitive CST816S touch and TF/LED/CDS/speaker peripherals.
* `boards/rk3568_4827S043R.json` – 4.3″ 480×272 ST7262 parallel display with resistive XPT2046 touch and TF card.
* `boards/rk3568_8048S070C.json` – 7.0″ 800×480 ST7262 parallel display with capacitive GT911 touch.

The legacy file `boards/rk3568_fcs866r_lcd480x272.json` remains as a minimal example
for a 4.3″ 480×272 LVDS display.  You can delete it if it is no longer needed.

## License

This repository is released under the MIT License.
