#ifndef TOUCHSCREEN_H
#define TOUCHSCREEN_H

#include <Arduino.h>
#include <lvgl.h>

/**
 * @brief Abstract base class for all touchscreen implementations.
 * 
 * This class defines a minimal interface for touchscreen input.
 * Any concrete touchscreen class (e.g., GigaDisplayTouch) should inherit from this
 * and implement the methods below.
 */
class TouchScreen {
public:
    virtual ~TouchScreen() {}

    /**
     * @brief Initialize the touchscreen hardware.
     * @return true if initialization succeeds.
     */
    virtual bool begin() = 0;

    /**
     * @brief Read a touch event.
     *        Integrates with LVGL by filling lv_indev_data_t.
     * @param data LVGL input data structure
     */
    virtual void read(lv_indev_data_t *data) = 0;

    /**
     * @brief Optional: Get raw X coordinate of the first touch.
     * @return X coordinate, or -1 if no touch.
     */
    virtual int getX() { return -1; }

    /**
     * @brief Optional: Get raw Y coordinate of the first touch.
     * @return Y coordinate, or -1 if no touch.
     */
    virtual int getY() { return -1; }

    /**
     * @brief Optional: Get touch pressure.
     * @return Pressure, or -1 if unsupported.
     */
    virtual int getPressure() { return -1; }
};

#endif // TOUCHSCREEN_H
