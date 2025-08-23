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

## Configuration and Setup

### PlatformIO Environment Configuration

You can create a `common.ini` file to centralize your PlatformIO project settings. This is particularly helpful for specifying library paths, upload ports, and compiler flags. C++11 is recommended to ensure full compatibility with the libraries in this repository.

```ini
[env:uno_r4_wifi]
lib_extra_dirs = ~/Projects/Mountjoy/RumpusArduinoLibrary/libraries
upload_port = /your/path/

; Ensure the compiler uses C++11, some libs require C++11
build_unflags = -std=gnu++98
build_flags = -std=gnu++11
```

> Note: Adjust `lib_extra_dirs` and `upload_port` to match your system and board.

---

### PlatformIO Project Configuration Header

Include a `config.h` in your project’s `include` folder. This header centralizes configuration options for WiFi, logging, and API details. Replace the placeholder values with your specific project settings.

```cpp
// config.h
#ifndef CONFIG_H
#define CONFIG_H

#define WIFI_SSID ""        // Your WiFi SSID
#define WIFI_PASS ""        // Your WiFi password
#define BAUD_RATE 9600      // Serial baud rate
#define LAN_IP ""           // Server IP for logging
#define API_PATH ""         // Path on server for API requests
#define SOURCE ""           // Source name for logging
#define TARGET_API ""       // Target API identifier
#define DB_ID ""            // Database or page ID
#define PORT 8000           // Default server port

#define DEBUG_LEVEL LOG_LEVEL_DEBUG  // Logging verbosity level

#endif
```

> 💡 Place `config.h` inside the `include` folder of your PlatformIO project to ensure it is available globally in your source code.

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
