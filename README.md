# PandaTouch DevKit

This repository serves as a base reference project for the PandaTouch Board, providing example configurations, code, and documentation to help you get started with hardware and LVGL development on the ESP32-S3 platform.

# Board Specifications

## MCU

- Model: ESP32-S3 (dual-core, Xtensa LX7 @ up to 240 MHz)
- Features: Wi-Fi (2.4 GHz), Bluetooth 5 LE, USB OTG
- Memory: 8 MB Octal PSRAM onboard

## Display

- Type: RGB parallel LCD
- Resolution: 800×480
- Color Depth: 16-bit (RGB565)
- Signals: PCLK, HSYNC, VSYNC, DE, DATA[15:0], Backlight

### LCD Pin Mapping (from `main/pinout.h`)

| Signal    | GPIO | Notes                     |
| --------- | ---- | ------------------------- |
| PCLK      | 5    | Pixel clock               |
| DE        | 38   | Data enable               |
| HSYNC     | –    | Not routed (DE mode used) |
| VSYNC     | –    | Not routed (DE mode used) |
| R3        | 6    | Data                      |
| R4        | 7    | Data                      |
| R5        | 8    | Data                      |
| R6        | 9    | Data                      |
| R7        | 10   | Data                      |
| G2        | 11   | Data                      |
| G3        | 12   | Data                      |
| G4        | 13   | Data                      |
| G5        | 14   | Data                      |
| G6        | 15   | Data                      |
| G7        | 16   | Data                      |
| B3        | 17   | Data                      |
| B4        | 18   | Data                      |
| B5        | 48   | Data                      |
| B6        | 47   | Data                      |
| B7        | 39   | Data                      |
| Backlight | 21   | PWM capable               |
| Reset     | 46   | LCD reset                 |

- Backlight control: PWM via LEDC on GPIO21

## Touch

- Controller: GT911 (capacitive touch)
- Interface: I²C0

| Signal | GPIO |
| ------ | ---- |
| SCL    | 1    |
| SDA    | 2    |
| IRQ    | 40   |
| RST    | 41   |

## I²C Initialization Guide

### GT911 Touch Controller (I²C0)

- Pins: SCL = GPIO1, SDA = GPIO2
- Bus: `I2C_NUM_0`
- Speed: 400 kHz

### AHT20 Humidity & Temperature Sensor (I²C1)

- Pins: SCL = GPIO3, SDA = GPIO4
- Bus: `I2C_NUM_1`
- Speed: 100 kHz

## USB

- Mode: USB OTG (device/host, software-controlled)
- Pins: D− GPIO19, D+ GPIO20 (ESP32-S3 built-in USB PHY)

## Other Peripherals

- PWM Backlight: GPIO21 via LEDC
- Connectivity: Wi-Fi, Bluetooth
- Available interfaces: UART, I²C, SPI, ADC, DAC, CAN, LEDC, MCPWM, RMT, etc.

---

# PlatformIO

This section provides the PlatformIO environment configuration for building and uploading firmware to the PandaTouch Board. PlatformIO is a cross-platform build system and IDE for embedded development, making it easy to manage dependencies, build flags, and board settings.

For more information and step-by-step guides, see:

- [PlatformIO Getting Started Guide](https://docs.platformio.org/en/latest/introduction.html)
- [PlatformIO VS Code Extension](https://platformio.org/install/ide?install=vscode)
- [PlatformIO ESP32 Documentation](https://docs.platformio.org/en/latest/boards/espressif32/esp32-s3-devkitc-1.html)

The example below sets up the ESP32-S3 with the Arduino framework, specifies the required libraries (LVGL, GT911 touch, and GFX), and configures build flags for PSRAM and LVGL settings. You can use this configuration in your `platformio.ini` file to build, upload, and monitor your firmware directly from PlatformIO.

````ini
[env:pandatouch]
platform = espressif32@6.9.0
framework = arduino
board = esp32-s3-devkitc-1
monitor_speed = 115200
lib_deps =
  lvgl/lvgl#9.3.0
  tamctec/TAMC_GT911@^1.0.2
  moononournation/GFX Library for Arduino@1.5.0

build_flags =
  -I include
  -DLV_CONF_PATH="${PROJECT_DIR}/include/lv_conf.h"
  -DBOARD_HAS_PSRAM
# PandaTouch DevKit

PandaTouch Arduino Template — glue code and examples for using the PandaTouch board (ESP32‑S3) with LVGL in PlatformIO/Arduino projects.

## Overview

This repository is a reference template for the PandaTouch DevKit (ESP32‑S3). It provides a minimal Arduino/PlatformIO project that initializes an 800×480 RGB parallel LCD, GT911 capacitive touch via I²C, and LVGL v9.3.0 for building embedded GUIs. The code includes a brightness demo, backlight PWM control via LEDC, and sample LVGL usage to help you get started quickly.

This project is aimed at embedded developers, makers, and engineers who want a working starting point for creating LVGL-based UIs on ESP32-S3 hardware with RGB parallel panels.

## Key features

- Ready-to-build PlatformIO template for PandaTouch (ESP32‑S3)
- LVGL v9.3.0 integration (graphics library)
- GT911 capacitive touch driver (I²C)
- RGB parallel LCD support (800×480, RGB565)
- PWM backlight control via LEDC with brightness persistence
- Partial LVGL draw buffers and PSRAM-aware configuration to save SRAM
- Minimal example demos (brightness slider, Hello World)

## Quick start (PlatformIO)

Prerequisites

- PlatformIO (VS Code recommended)
- USB drivers for your OS (if required by your USB-serial adapter)
- PandaTouch DevKit (ESP32-S3) with panel and GT911 wired

Build and upload

```bash
# Build
pio run

# Upload (replace /dev/ttyUSB0 with your port)
pio run --target upload --environment pandatouch --upload-port /dev/ttyUSB0

# Serial monitor (115200)
pio device monitor -b 115200
````

Notes

- The repository `platformio.ini` pins `platform = espressif32@6.12.0` (use the exact version in the file to match precompiled SDK lines).
- The `pandatouch` environment provides `lib_deps` for LVGL, GT911 and the GFX library.

## Usage

- Call `pt_setup_display()` in `setup()` to initialize the display, touch and LVGL.
- Create LVGL objects (buttons, labels, sliders) as usual.
- Call `pt_loop_display()` inside `loop()` to run LVGL tasks and process touch input.

Minimal example (summary)

- `src/main.cpp` calls `pt_setup_display()` and `pt_demo_create_brightness_demo()` on boot.
- `src/pt/pt_display.h` exposes `pt_set_backlight()`, `pt_init_backlight()`, `pt_setup_display()`, and `pt_loop_display()`.
- `src/pt_demo.h` builds a simple black screen with a brightness slider which calls `pt_set_backlight(percent, true)`.

## Board specifications

### MCU

- Model: ESP32-S3 (dual-core, Xtensa LX7 @ up to 240 MHz)
- Features: Wi‑Fi (2.4 GHz), Bluetooth 5 LE, USB OTG
- Memory: 8 MB Octal PSRAM onboard

### Display

- Type: RGB parallel LCD
- Resolution: 800×480
- Color Depth: 16-bit (RGB565)
- Signals: PCLK, HSYNC, VSYNC, DE, DATA[15:0], Backlight

#### LCD Pin Mapping (from `main/pinout.h`)

| Signal    | GPIO | Notes                     |
| --------- | ---- | ------------------------- |
| PCLK      | 5    | Pixel clock               |
| DE        | 38   | Data enable               |
| HSYNC     | –    | Not routed (DE mode used) |
| VSYNC     | –    | Not routed (DE mode used) |
| R3        | 6    | Data                      |
| R4        | 7    | Data                      |
| R5        | 8    | Data                      |
| R6        | 9    | Data                      |
| R7        | 10   | Data                      |
| G2        | 11   | Data                      |
| G3        | 12   | Data                      |
| G4        | 13   | Data                      |
| G5        | 14   | Data                      |
| G6        | 15   | Data                      |
| G7        | 16   | Data                      |
| B3        | 17   | Data                      |
| B4        | 18   | Data                      |
| B5        | 48   | Data                      |
| B6        | 47   | Data                      |
| B7        | 39   | Data                      |
| Backlight | 21   | PWM capable               |
| Reset     | 46   | LCD reset                 |

- Backlight control: PWM via LEDC on GPIO21

### Touch

- Controller: GT911 (capacitive touch)
- Interface: I²C0

| Signal | GPIO |
| ------ | ---- |
| SCL    | 1    |
| SDA    | 2    |
| IRQ    | 40   |
| RST    | 41   |

## I²C initialization guide

### GT911 Touch Controller (I²C0)

- Pins: SCL = GPIO1, SDA = GPIO2
- Bus: `I2C_NUM_0`
- Speed: 400 kHz

### AHT20 Humidity & Temperature Sensor (I²C1)

- Pins: SCL = GPIO3, SDA = GPIO4
- Bus: `I2C_NUM_1`
- Speed: 100 kHz

## USB

- Mode: USB OTG (device/host, software-controlled)
- Pins: D− GPIO19, D+ GPIO20 (ESP32-S3 built-in USB PHY)

## Other peripherals

- PWM Backlight: GPIO21 via LEDC
- Connectivity: Wi‑Fi, Bluetooth
- Available interfaces: UART, I²C, SPI, ADC, DAC, CAN, LEDC, MCPWM, RMT, etc.

## Build configuration notes

- `platformio.ini` contains the `pandatouch` environment and `lib_deps` for LVGL, GT911 and GFX.
- `build_flags` set `-DBOARD_HAS_PSRAM` to enable PSRAM-aware behavior and `-DLV_CONF_PATH` to point to `include/lv_conf.h`.

## Troubleshooting

Common issues and fixes

- Build fails (missing LVGL or GFX): run `pio update` and confirm the `lib_deps` lines in `platformio.ini`.
- Out of memory or LVGL allocation failures: ensure PSRAM is enabled (`BOARD_HAS_PSRAM`) and review `include/lv_conf.h` buffer settings. The project uses partial buffering to reduce SRAM usage.
- Touch not working: confirm GT911 wiring (SDA/SCL pins), IRQ and RST wiring, and confirm the device appears on the bus.
- Display shows white/no image: check reset pin wiring, panel power, and ensure `pt_gfx.begin()` runs without errors.
- Upload errors on macOS: check the serial port name (e.g. `/dev/cu.usbserial-XXXX`) and close other serial monitors.

## Contribution

Contributions are welcome. Please open an issue for large changes, or submit a pull request. Use feature branches and describe the change in the PR.

## License

This repository does not include an explicit license file. If you want to permit broad reuse, consider the MIT License. Tell me which license you prefer and I will add a `LICENSE` file.

## Resources

- LVGL documentation: https://docs.lvgl.io/
- PlatformIO: https://platformio.org/
- LCD datasheet: `docs/QX05ZJGI70N-05545Y.pdf`

---

If you want, I can now:

1. Add an explicit `LICENSE` file (MIT by default).
2. Extract the pin defines from `pt/pt_board.h` and replace the table with the auto-generated definitive mapping.
3. Add screenshot placeholders under `docs/screenshots` and update README with images.

Which of these shall I do next?
