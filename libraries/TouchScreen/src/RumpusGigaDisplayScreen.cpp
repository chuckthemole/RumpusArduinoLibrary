#include "RumpusGigaDisplayScreen.h"

// Singleton instance
RumpusGigaDisplayScreen *RumpusGigaDisplayScreen::_instance = nullptr;

// ------------------ Constructor / Destructor ------------------
RumpusGigaDisplayScreen::RumpusGigaDisplayScreen(uint16_t horRes, uint16_t verRes, uint16_t bufHeight)
    : _horRes(horRes), _verRes(verRes), _bufHeight(bufHeight), _buf1(nullptr), _buf2(nullptr)
{
    if (_bufHeight == 0)
        _bufHeight = 100; // safe default
    _instance = this;
}

RumpusGigaDisplayScreen::~RumpusGigaDisplayScreen()
{
    delete[] _buf1;
    delete[] _buf2;
}

// ------------------ Initialization ------------------
bool RumpusGigaDisplayScreen::begin()
{
    Serial.println("Display init...");
    lv_init();
    _display.begin();
    _touch.begin();

    // Allocate buffers AFTER lv_init()
    _buf1 = new lv_color_t[_horRes * _bufHeight];
    _buf2 = new lv_color_t[_horRes * _bufHeight];
    lv_disp_draw_buf_init(&_draw_buf, _buf1, _buf2, _horRes * _bufHeight);

    // Display driver
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = _horRes;
    disp_drv.ver_res = _verRes;
    disp_drv.flush_cb = flush_cb;
    disp_drv.draw_buf = &_draw_buf;
    lv_disp_drv_register(&disp_drv);

    // Input driver
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touch_read_cb;
    lv_indev_drv_register(&indev_drv);

    Serial.println("LVGL init done.");
    return true;
}

// ------------------ Main LVGL Loop ------------------
void RumpusGigaDisplayScreen::loop()
{
    lv_timer_handler(); // process LVGL tasks (animations, input, redraws)
    delay(5);           // small delay to avoid busy loop
}

// ------------------ LVGL Callbacks ------------------
void RumpusGigaDisplayScreen::flush_cb(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    if (!_instance)
        return;
    _instance->_display.drawRGBBitmap(area->x1, area->y1, (uint16_t *)color_p,
                                      area->x2 - area->x1 + 1, area->y2 - area->y1 + 1);
    lv_disp_flush_ready(disp_drv);
}

void RumpusGigaDisplayScreen::touch_read_cb(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
    if (!_instance)
        return;

    GDTpoint_t points[5];
    uint8_t n = _instance->_touch.getTouchPoints(points);

    if (n > 0)
    {
        data->state = LV_INDEV_STATE_PRESSED;
        data->point.x = points[0].x;
        data->point.y = points[0].y;
    }
    else
    {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

// ------------------ Widgets (with optional font) ------------------

lv_obj_t *RumpusGigaDisplayScreen::createLabel(const char *text,
                                               lv_obj_t *parent,
                                               const lv_font_t *font)
{
    if (!parent)
        parent = lv_scr_act();

    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_text(label, text);

    if (font)
    {
        lv_style_t *style = new lv_style_t;
        lv_style_init(style);
        lv_style_set_text_font(style, font);
        lv_obj_add_style(label, style, 0);
    }

    return label;
}

lv_obj_t *RumpusGigaDisplayScreen::createCenteredLabel(const char *text,
                                                       lv_obj_t *parent,
                                                       const lv_font_t *font)
{
    lv_obj_t *label = createLabel(text, parent, font);
    lv_obj_center(label);
    return label;
}

lv_obj_t *RumpusGigaDisplayScreen::createButton(const char *text,
                                                lv_obj_t *parent,
                                                lv_event_cb_t event_cb,
                                                const lv_font_t *font)
{
    if (!parent)
        parent = lv_scr_act();

    lv_obj_t *btn = lv_btn_create(parent);
    if (event_cb)
        lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, nullptr);

    // Label inside button
    lv_obj_t *label = createLabel(text, btn, font);
    lv_obj_center(label);

    return btn;
}

lv_obj_t *RumpusGigaDisplayScreen::createFlexContainer(lv_obj_t *parent,
                                                       lv_flex_flow_t flow)
{
    if (!parent)
        parent = lv_scr_act();

    lv_obj_t *cont = lv_obj_create(parent);
    lv_obj_set_size(cont, lv_pct(100), lv_pct(100));
    lv_obj_set_flex_flow(cont, flow);

    return cont;
}

lv_obj_t *RumpusGigaDisplayScreen::createCenterLayout(lv_obj_t *parent,
                                                      lv_flex_flow_t flow)
{
    lv_obj_t *cont = createFlexContainer(parent, flow);
    lv_obj_center(cont);
    return cont;
}

lv_obj_t *RumpusGigaDisplayScreen::createSlider(lv_obj_t *parent,
                                                int16_t min,
                                                int16_t max,
                                                int16_t value,
                                                lv_event_cb_t event_cb)
{
    if (!parent)
        parent = lv_scr_act();

    lv_obj_t *slider = lv_slider_create(parent);
    lv_slider_set_range(slider, min, max);
    lv_slider_set_value(slider, value, LV_ANIM_OFF);

    if (event_cb)
        lv_obj_add_event_cb(slider, event_cb, LV_EVENT_VALUE_CHANGED, nullptr);

    return slider;
}

lv_obj_t *RumpusGigaDisplayScreen::createSwitch(lv_obj_t *parent,
                                                bool on,
                                                lv_event_cb_t event_cb)
{
    if (!parent)
        parent = lv_scr_act();

    lv_obj_t *sw = lv_switch_create(parent);
    if (on)
        lv_obj_add_state(sw, LV_STATE_CHECKED);

    if (event_cb)
        lv_obj_add_event_cb(sw, event_cb, LV_EVENT_VALUE_CHANGED, nullptr);

    return sw;
}

lv_obj_t *RumpusGigaDisplayScreen::createProgressBar(lv_obj_t *parent,
                                                     int16_t min,
                                                     int16_t max,
                                                     int16_t value)
{
    if (!parent)
        parent = lv_scr_act();

    lv_obj_t *bar = lv_bar_create(parent);
    lv_bar_set_range(bar, min, max);
    lv_bar_set_value(bar, value, LV_ANIM_OFF);

    return bar;
}

lv_obj_t *RumpusGigaDisplayScreen::createTextArea(lv_obj_t *parent,
                                                  const char *text,
                                                  lv_event_cb_t event_cb,
                                                  const lv_font_t *font)
{
    if (!parent)
        parent = lv_scr_act();

    lv_obj_t *ta = lv_textarea_create(parent);
    lv_textarea_set_text(ta, text);

    if (font)
    {
        lv_style_t *style = new lv_style_t;
        lv_style_init(style);
        lv_style_set_text_font(style, font);
        lv_obj_add_style(ta, style, 0);
    }

    if (event_cb)
        lv_obj_add_event_cb(ta, event_cb, LV_EVENT_VALUE_CHANGED, nullptr);

    return ta;
}
