#include "RumpusGigaDisplayScreen.h"

// Static instance pointer for callbacks
static RumpusGigaDisplayScreen* _instance = nullptr;

// Constructor
RumpusGigaDisplayScreen::RumpusGigaDisplayScreen(uint16_t horRes, uint16_t verRes, uint16_t bufHeight)
    : _horRes(horRes), _verRes(verRes), _bufHeight(bufHeight),
      _buf1(nullptr), _buf2(nullptr)
{
    // Set the static instance pointer for callbacks
    _instance = this;
}

// Begin / initialize everything
bool RumpusGigaDisplayScreen::begin() {
    _display.begin();
    _touch.begin();
    lv_init();

    // Allocate buffers
    _buf1 = new lv_color_t[_horRes * _bufHeight];
    _buf2 = new lv_color_t[_horRes * _bufHeight];
    lv_disp_draw_buf_init(&_draw_buf, _buf1, _buf2, _horRes * _bufHeight);

    // Initialize display driver
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = _horRes;
    disp_drv.ver_res = _verRes;
    disp_drv.flush_cb = flush_cb;
    disp_drv.draw_buf = &_draw_buf;
    lv_disp_drv_register(&disp_drv);

    // Initialize input device driver
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touch_read_cb;
    lv_indev_drv_register(&indev_drv);

    return true;
}

// LVGL loop function
void RumpusGigaDisplayScreen::loop() {
    lv_timer_handler();
}

// Flush callback
void RumpusGigaDisplayScreen::flush_cb(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p) {
    if (!_instance) return;

    _instance->_display.drawRGBBitmap(
        area->x1, area->y1,
        (uint16_t*)color_p,
        area->x2 - area->x1 + 1,
        area->y2 - area->y1 + 1
    );

    lv_disp_flush_ready(disp_drv);
}

// Touch read callback
void RumpusGigaDisplayScreen::touch_read_cb(lv_indev_drv_t *drv, lv_indev_data_t *data) {
    if (!_instance) return;

    GDTpoint_t points[5];
    uint8_t n = _instance->_touch.getTouchPoints(points);

    if (n > 0) {
        data->state = LV_INDEV_STATE_PRESSED;
        data->point.x = points[0].x;
        data->point.y = points[0].y;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

// Create a label helper
lv_obj_t* RumpusGigaDisplayScreen::createLabel(const char* text, int x, int y, lv_align_t align) {
    lv_obj_t* label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, text);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_40, 0);
    lv_obj_align(label, align, x, y);
    _lastLabel = label;
    return label;
}

// Create a button helper
lv_obj_t* RumpusGigaDisplayScreen::createButton(const char* text, int x, int y, lv_event_cb_t event_cb) {
    lv_obj_t* btn = lv_btn_create(lv_scr_act());
    lv_obj_set_size(btn, 150, 60);
    lv_obj_align(btn, LV_ALIGN_CENTER, x, y);
    if (event_cb) {
        lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, nullptr);
    }

    lv_obj_t* label = lv_label_create(btn);
    lv_label_set_text(label, text);
    lv_obj_center(label);

    return btn;
}
