# Bare minimum sketch for the Sunton aka Cheap Yellow Display (CYD) boards. ESP32_1732S019N/C, 2424S012N/C, 2432S024R/C/N, 2432S028R, 2432S032N/R/C, 3248S035R/C, 4827S043R/C, 4848S040C, 8048S050N/C and 8048S070N/C

For [PlatformIO](https://platformio.org/)

[![Platform IO CI](https://github.com/rzeldent/esp32-smartdisplay-demo/actions/workflows/main.yml/badge.svg)](https://github.com/rzeldent/esp32-smartdisplay-demo/actions/workflows/main.yml)

This is a demo application for the [esp32-smartdisplay](https://github.com/rzeldent/esp32-smartdisplay) library that is intended to be used in [PlatformIO](https://platformio.org/).
See [https://github.com/rzeldent/esp32-smartdisplay](https://github.com/rzeldent/esp32-smartdisplay/) for more information about the driver library.

> [!WARNING]
> Do not forget to clone this repository with submodules: `git clone --recurse-submodules`!
> This is required to also have the latest version of the [Sunton boards definitions](https://github.com/rzeldent/platformio-espressif32-sunton).

Example with sound! (if WiFi credentials are provided and speaker attached)

![Example](assets/PXL_20231130_225143662.jpg)

## Version history

- December 2024
  - Use EEZ Studio for designing the GUI [https://www.envox.eu/studio/studio-introduction/](https://www.envox.eu/studio/studio-introduction/)
- August 2024
  - LVGL 9.2
  - New boards
- July 2024
  - LVGL 9.1
  - Use release 2.0.10
- June 2024
  - Update SquareLine project to 1.4.1
- July 2024
  - LVGL 9.1
  - Use release 2.0.10
- June 2024
  - Update SquareLine project to 1.4.1
- March 2024
  - Added rotate button
  - Removed radio (and wifi dependencies)
- December 2023
  - Release 2.0.0
  - Updated demo with sound
- November 2023
  - Use of new library
  - Updated demo application with sound
- March 2023
  - Demo application created
- October 2023
  - Updated UI using [SquareLine Studio](https://squareline.io). This is a graphical UI design tool.
