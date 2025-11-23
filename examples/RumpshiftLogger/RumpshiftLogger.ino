#include <Arduino.h>
#include <RumpshiftLogger.h>

// Create the logger
RumpshiftLogger logger(115200, LOG_LEVEL_DEBUG, true);

// Example: a global LVGL label (or your own UI function)
lv_obj_t *ui_LogLabel;

// This function will be called every time the logger prints something
void handleLogCallback(const String &line)
{
    // Example: print to LVGL
    if (ui_LogLabel)
    {
        lv_label_set_text(ui_LogLabel, line.c_str());
    }

    // You can also do anything else here:
    // - Send logs over WiFi
    // - Buffer them into a file
    // - Trigger on specific messages
}

void setup()
{
    Serial.begin(115200);

    // Initialize LVGL, build UI, etc.
    // ui_LogLabel = lv_label_create(...);

    // Register the callback
    logger.setLogCallback(handleLogCallback);

    // Log something
    logger.info("System startup complete.");
    logger.debug("WiFi subsystem initializing...");
}

void loop()
{
    logger.warn("This is a warning that will appear in LVGL and Serial.");
    delay(3000);
}
