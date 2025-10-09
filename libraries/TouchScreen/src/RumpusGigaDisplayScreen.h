#ifndef RUMPUS_GIGA_DISPLAY_SCREEN_H
#define RUMPUS_GIGA_DISPLAY_SCREEN_H

#include <Arduino.h>
#include <functional>
#include <lvgl.h>
#include "RumpusLvglFont.h"
#include "RumpshiftLogger.h"
#include <Arduino_GigaDisplay_GFX.h>
#include <Arduino_GigaDisplayTouch.h>

/**
 * @class RumpusGigaDisplayScreen
 * @brief Wrapper around LVGL for the Arduino Giga Display.
 *
 * This class provides:
 * - Display initialization and loop handling
 * - Theme management (light/dark)
 * - Convenient widget creation (buttons, labels, sliders, etc.)
 * - Debounced button support
 *
 * Uses singleton pattern internally for LVGL callbacks.
 */
class RumpusGigaDisplayScreen
{
public:
    /**
     * @brief Construct a new display screen.
     * @param horRes Horizontal resolution of the screen (default 480)
     * @param verRes Vertical resolution of the screen (default 800)
     * @param bufHeight Height of LVGL draw buffer (default 0 = auto 100)
     */
    RumpusGigaDisplayScreen(
        RumpshiftLogger *logger,
        uint16_t horRes = 480,
        uint16_t verRes = 800,
        uint16_t bufHeight = 0);

    /** @brief Destructor frees allocated buffers */
    ~RumpusGigaDisplayScreen();

    /** @brief Initialize display, touch, and LVGL */
    bool begin();

    /** @brief Run the LVGL timer handler (call in Arduino loop) */
    void loop();

    /** @brief Apply a simple predefined light theme */
    void applySimpleTheme();

    /** @brief Toggle between light and dark mode */
    void toggleDarkMode();

    /** @brief Remove all objects from the active screen */
    void clear()
    {
        lv_obj_clean(lv_scr_act());
    }

    /** @brief Load a new LVGL screen */
    void loadNewScreen(lv_obj_t *new_screen)
    {
        lv_scr_load(new_screen);
    }

    /** @brief Get the currently active LVGL screen */
    lv_obj_t *getActiveScreen()
    {
        return lv_scr_act();
    }

    // ------------------ Widget creation helpers ------------------

    lv_obj_t *createLabel(
        const char *text,
        lv_obj_t *parent = nullptr,
        const lv_font_t *font = DEFAULT_FONT_LARGE);

    lv_obj_t *createCenteredLabel(
        const char *text,
        lv_obj_t *parent = nullptr,
        const lv_font_t *font = DEFAULT_FONT_LARGE);

    lv_obj_t *createButton(
        const char *text,
        lv_obj_t *parent = nullptr,
        lv_event_cb_t event_cb = nullptr,
        const lv_font_t *font = DEFAULT_FONT_LARGE);

    lv_obj_t *createDebouncedButton(
        const char *text,
        lv_obj_t *parent,
        std::function<void()> cb,
        const lv_font_t *font = DEFAULT_FONT_LARGE);

    lv_obj_t *createGridContainer(
        lv_obj_t *parent = nullptr,
        const lv_coord_t *row_dsc = nullptr,
        const lv_coord_t *col_dsc = nullptr,
        uint8_t row_count = 0,
        uint8_t col_count = 0,
        lv_align_t align = LV_ALIGN_CENTER);

    lv_obj_t *createFlexContainer(
        lv_obj_t *parent = nullptr,
        lv_flex_flow_t flow = LV_FLEX_FLOW_COLUMN);

    lv_obj_t *createCenterLayout(
        lv_obj_t *parent = nullptr,
        lv_flex_flow_t flow = LV_FLEX_FLOW_COLUMN);

    lv_obj_t *createSlider(
        lv_obj_t *parent = nullptr,
        int16_t min = 0,
        int16_t max = 100,
        int16_t value = 0,
        lv_event_cb_t event_cb = nullptr);

    lv_obj_t *createSwitch(
        lv_obj_t *parent = nullptr,
        bool on = false,
        lv_event_cb_t event_cb = nullptr);

    lv_obj_t *createProgressBar(
        lv_obj_t *parent = nullptr,
        int16_t min = 0,
        int16_t max = 100,
        int16_t value = 0);

    lv_obj_t *createTextArea(
        lv_obj_t *parent = nullptr,
        const char *text = "",
        lv_event_cb_t event_cb = nullptr,
        const lv_font_t *font = DEFAULT_FONT_LARGE);

private:
    RumpshiftLogger *_logger = nullptr; ///< Optional logger for debug/info messages
    uint16_t _horRes;                   ///< Screen horizontal resolution
    uint16_t _verRes;                   ///< Screen vertical resolution
    uint16_t _bufHeight;                ///< LVGL draw buffer height

    uint32_t _lastButtonPress = 0;        ///< For debounced buttons
    const uint32_t _buttonCooldown = 250; ///< Button debounce time in milliseconds

    lv_color_t *_buf1;            ///< LVGL draw buffer 1
    lv_color_t *_buf2;            ///< LVGL draw buffer 2
    lv_disp_draw_buf_t _draw_buf; ///< LVGL draw buffer descriptor

    GigaDisplay_GFX _display;        ///< Hardware display interface
    Arduino_GigaDisplayTouch _touch; ///< Touch input interface

    bool _darkMode = false;     ///< Tracks current theme mode
    lv_disp_t *_disp = nullptr; ///< LVGL display pointer

    static RumpusGigaDisplayScreen *_instance; ///< Singleton instance for callbacks

    // ------------------ LVGL callback functions ------------------
    static void flush_cb(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);
    static void touch_read_cb(lv_indev_drv_t *drv, lv_indev_data_t *data);
};

#endif // RUMPUS_GIGA_DISPLAY_SCREEN_H
