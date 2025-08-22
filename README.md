# RumpusArduinoLibrary

A collection of reusable Arduino libraries for PlatformIO projects, maintained by Chuck Thomas.

Current modules:
- `ArduinoBoardLED`: Simple control for onboard Arduino LEDs.
- `CommonArduinoHelper`: Shared helper utilities for multiple projects.
- `ElectricEye`: Light or sensor-based module (for detecting input).
- `LCDWrapper`: Wrapper around common LCD display drivers.
- `LogHttp`: Logging module with HTTP support.
- `LogoLED`: Controls a logo/status LED.
- `MessageScroller`: Scrolls text across an attached display.
- `OTA`: Over-the-air update support.
- `RelayTester`: Utility for testing relay modules.
- `RumpshiftLogger`: General-purpose logging framework.
- `UserSelector`: Abstracts user input/selection (serial, buttons, etc.).
- `WiFiNetworkManager`: Simplifies WiFi connection setup.

---

## Repository Structure

RumpusArduinoLibrary/
├── libraries/                # All libraries live here
│   ├── WiFiNetworkManager/
│   │   └── src/...
│   └── LogoLED/
│       └── src/...
│
├── examples/           # Usage demos for each library
│   ├── WiFiNetworkManager_Connect/
│   └── LogoLED_Blink/
│
├── tests/              # Automated + visual tests
│   ├── unit/           # Logic/unit tests (Unity)
│   │   ├── test_WiFiNetworkManager/
│   │   └── test_LogoLED/
│   │
│   └── visual/         # Hardware/visual tests (LEDs, displays, etc.)
│       ├── test_leds/
│       └── test_wifi/
│
└── README.md

---

## Using in PlatformIO Projects

Instead of symlinking, simply point PlatformIO at the `lib/` folder with `lib_extra_dirs`.

In your project's `platformio.ini`:

[env:uno_r4_wifi]
platform = renesas-ra
board = uno_r4_wifi
framework = arduino

lib_extra_dirs = /Users/PATH_TO_YOUR_LIB_FOLDER/RumpusArduinoLibrary/lib

Then you can include your libraries like normal:

#include <WiFiNetworkManager.h>
#include <LogoLED.h>

PlatformIO will automatically find them.

---

## Testing

### 1. Unit Tests
- Located in `tests/unit/`
- Use the Unity test framework
- Run locally (native) or on a board:

pio test -e uno

### 2. Visual Tests
- Located in `tests/visual/`
- These behave like sketches (e.g. blink an LED, print to serial)
- Run on target hardware:

pio test -e uno -f test_leds

---

## Examples
- Located in `examples/`
- Minimal usage sketches for each library
- Recognized automatically by the Arduino IDE
- Great for reference and demos

---

## Summary
- Keep all libraries under `lib/`
- Use `lib_extra_dirs` in PlatformIO projects to include them
- `examples/` → usage demos
- `tests/unit/` → automated tests
- `tests/visual/` → hardware/manual tests
