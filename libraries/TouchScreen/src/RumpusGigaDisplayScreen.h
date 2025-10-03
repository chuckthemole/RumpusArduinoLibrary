#ifndef RUMPUS_GIGA_DISPLAY_SCREEN_H
#define RUMPUS_GIGA_DISPLAY_SCREEN_H

#include <Arduino.h>
#include <lvgl.h>
#include <Arduino_GigaDisplay_GFX.h>
#include <Arduino_GigaDisplayTouch.h>

#ifndef RUMPUS_GIGA_FONT
#define RUMPUS_GIGA_FONT lv_font_montserrat_40
#endif

LV_FONT_DECLARE(RUMPUS_GIGA_FONT);

class RumpusGigaDisplayScreen
{
public:
    RumpusGigaDisplayScreen(uint16_t horRes = 480, uint16_t verRes = 800, uint16_t bufHeight = 0);
    ~RumpusGigaDisplayScreen();

    bool begin();
    void loop();

    // Screen helpers
    void clear()
    {
        // remove all objects from the active screen
        lv_obj_clean(lv_scr_act());
    }
    void loadNewScreen(lv_obj_t *new_screen)
    {
        lv_scr_load(new_screen);
    }

    // Widgets
    lv_obj_t *createLabel(const char *text,
                          lv_obj_t *parent = nullptr,
                          const lv_font_t *font = &RUMPUS_GIGA_FONT);

    lv_obj_t *createCenteredLabel(const char *text,
                                  lv_obj_t *parent = nullptr,
                                  const lv_font_t *font = &RUMPUS_GIGA_FONT);

    lv_obj_t *createButton(const char *text,
                           lv_obj_t *parent = nullptr,
                           lv_event_cb_t event_cb = nullptr,
                           const lv_font_t *font = &RUMPUS_GIGA_FONT);

    lv_obj_t *createFlexContainer(lv_obj_t *parent = nullptr,
                                  lv_flex_flow_t flow = LV_FLEX_FLOW_COLUMN);

    lv_obj_t *createCenterLayout(lv_obj_t *parent = nullptr,
                                 lv_flex_flow_t flow = LV_FLEX_FLOW_COLUMN);

    lv_obj_t *createSlider(lv_obj_t *parent = nullptr,
                           int16_t min = 0,
                           int16_t max = 100,
                           int16_t value = 0,
                           lv_event_cb_t event_cb = nullptr);

    lv_obj_t *createSwitch(lv_obj_t *parent = nullptr,
                           bool on = false,
                           lv_event_cb_t event_cb = nullptr);

    lv_obj_t *createProgressBar(lv_obj_t *parent = nullptr,
                                int16_t min = 0,
                                int16_t max = 100,
                                int16_t value = 0);

    lv_obj_t *createTextArea(lv_obj_t *parent = nullptr,
                             const char *text = "",
                             lv_event_cb_t event_cb = nullptr,
                             const lv_font_t *font = &RUMPUS_GIGA_FONT);

private:
    uint16_t _horRes;
    uint16_t _verRes;
    uint16_t _bufHeight;

    lv_color_t *_buf1;
    lv_color_t *_buf2;
    lv_disp_draw_buf_t _draw_buf;

    GigaDisplay_GFX _display;
    Arduino_GigaDisplayTouch _touch;

    // Singleton instance for callbacks
    static RumpusGigaDisplayScreen *_instance;

    // LVGL callbacks
    static void flush_cb(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);
    static void touch_read_cb(lv_indev_drv_t *drv, lv_indev_data_t *data);
};

#endif
