# RumpusArduinoLibrary

A collection of reusable Arduino libraries for PlatformIO projects, maintained by Chuck Thomas.

Current modules:
- `WiFiHelper`: Simplifies WiFi connection setup for ESP-based boards.
- `LogoLED`: Controls an onboard status LED or logo indicator.

---

## Installation Instructions

This library is organized as a **monorepo**. Each module (e.g. `WiFiHelper`, `LogoLED`) is a self-contained PlatformIO-compatible library stored in its own subdirectory.

Since PlatformIO does not support importing subfolders directly via `lib_deps`, you must **clone this repo and symlink** individual libraries into your project manually.

### Step-by-Step Setup

1. **Clone this repository** somewhere on your system (outside your PlatformIO project directory):

   ```bash
   git clone https://github.com/chuckthemole/RumpusArduinoLibrary.git
   ```

2. **Create a symbolic link** from the library you want (e.g. WiFiHelper) into your PlatformIO project’s lib/ directory.
    ```bash
    cd your-platformio-project
    mkdir -p lib
    ```
    **Symlink the WiFiHelper library**
    ```bash
    ln -s /full/path/to/RumpusArduinoLibrary/WiFiHelper lib/WiFiHelper
    ```
    **Optionally symlink LogoLED too**
    ```bash
    ln -s /full/path/to/RumpusArduinoLibrary/LogoLED lib/LogoLED
    ```

3. **Include headers in your code** like normal:
    ```bash
    #include <WiFiHelper.h>
    #include <LogoLED.h>
    ```

1. **Compile and run your project!** PlatformIO will treat the symlinked folders as local libraries.