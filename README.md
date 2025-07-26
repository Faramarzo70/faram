
# RK3568‑WF EVB Kit & FCS866R Board Definitions

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

## How to use these definitions

These JSON files are provided as a reference for integrating LVGL applications on the RK3568
platform.  They include compile‑time flags for display resolution and buffer sizes, and
indicate that the board supports Wi‑Fi 6 and Bluetooth 5.2 via SDIO and PCM/UART.  The
definitions are **not directly supported by PlatformIO**, because PlatformIO does not
currently support Linux hosts.  Instead, you should use these files as documentation for
configuring your build system (e.g. Yocto or Buildroot) and LVGL application on the RK3568.

## Files in this repository

* `boards/rk3568_fcs866r_lcd480x272.json` – example board definition for a 4.3″ 480×272 LVDS
  panel connected to the RK3568.

## License

This repository is released under the MIT License.
