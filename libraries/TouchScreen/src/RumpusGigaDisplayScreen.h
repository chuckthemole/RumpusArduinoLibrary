#ifndef RUMPUS_GIGA_DISPLAY_SCREEN_H
#define RUMPUS_GIGA_DISPLAY_SCREEN_H

#include <Arduino.h>
#include <lvgl.h>
#include <Arduino_GigaDisplay_GFX.h>
#include <Arduino_GigaDisplayTouch.h>

LV_FONT_DECLARE(lv_font_montserrat_40);

class RumpusGigaDisplayScreen {
public:
    // Constructor: optionally set buffer height
    RumpusGigaDisplayScreen(uint16_t horRes = 480, uint16_t verRes = 800, uint16_t bufHeight = 40);

    // Initialize everything
    bool begin();

    // LVGL loop function
    void loop();

    // Convenience helpers
    lv_obj_t* createLabel(const char* text, int x = 0, int y = 0, lv_align_t align = LV_ALIGN_TOP_MID);
    lv_obj_t* createButton(const char* text, int x, int y, lv_event_cb_t event_cb = nullptr);

private:
    uint16_t _horRes;
    uint16_t _verRes;
    uint16_t _bufHeight;

    // Double buffer
    lv_color_t* _buf1;
    lv_color_t* _buf2;
    lv_disp_draw_buf_t _draw_buf;

    GigaDisplay_GFX _display;
    Arduino_GigaDisplayTouch _touch;

    // Internal flush callback
    static void flush_cb(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);

    // Internal touch read callback
    static void touch_read_cb(lv_indev_drv_t *drv, lv_indev_data_t *data);

    // Keep track of created labels/buttons if needed
    lv_obj_t* _lastLabel = nullptr;
};

#endif // RUMPUS_GIGA_DISPLAY_SCREEN_H
