#include "RumpusGigaDisplayScreen.h"

// Static instance pointer for callbacks
static RumpusGigaDisplayScreen *_instance = nullptr;

// Constructor
RumpusGigaDisplayScreen::RumpusGigaDisplayScreen(uint16_t horRes, uint16_t verRes, uint16_t bufHeight)
    : _horRes(horRes), _verRes(verRes), _bufHeight(bufHeight),
      _buf1(nullptr), _buf2(nullptr)
{
    _instance = this;
}

// Begin / initialize everything
bool RumpusGigaDisplayScreen::begin()
{
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

// LVGL loop
void RumpusGigaDisplayScreen::loop()
{
    lv_timer_handler();
}

// Flush callback
void RumpusGigaDisplayScreen::flush_cb(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    if (!_instance)
        return;

    _instance->_display.drawRGBBitmap(
        area->x1, area->y1,
        (uint16_t *)color_p,
        area->x2 - area->x1 + 1,
        area->y2 - area->y1 + 1);

    lv_disp_flush_ready(disp_drv);
}

// Touch read callback
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

lv_obj_t *RumpusGigaDisplayScreen::createLabel(const char *text, lv_obj_t *parent)
{
    if (!parent)
        parent = lv_scr_act();

    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_40, 0);
    lv_obj_set_style_text_color(label, lv_color_black(), 0);
    lv_obj_set_width(label, lv_pct(100)); // wrap in flex container

    // Optional: add bottom padding for spacing in flex container
    lv_obj_set_style_pad_bottom(label, 10, 0);

    return label;
}

lv_obj_t *RumpusGigaDisplayScreen::createCenteredLabel(const char *text, lv_obj_t *parent)
{
    if (!parent)
        parent = lv_scr_act();

    lv_obj_t *box = lv_obj_create(parent);
    lv_obj_set_flex_flow(box, LV_FLEX_FLOW_ROW);
    lv_obj_set_width(box, lv_pct(100));
    lv_obj_clear_flag(box, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *label = lv_label_create(box);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_40, 0);
    lv_obj_center(label);

    return label;
}

// Create a button
lv_obj_t *RumpusGigaDisplayScreen::createButton(const char *text, lv_obj_t *parent, lv_event_cb_t event_cb)
{
    if (!parent)
        parent = lv_scr_act();
    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_set_size(btn, 150, 60);
    if (event_cb)
        lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, nullptr);

    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, text);
    lv_obj_center(label);
    return btn;
}

// Create a flex container with padding + scrollable
lv_obj_t *RumpusGigaDisplayScreen::createFlexContainer(lv_obj_t *parent, lv_flex_flow_t flow)
{
    if (!parent)
        parent = lv_scr_act();

    lv_obj_t *cont = lv_obj_create(parent);

    // Make background transparent
    lv_obj_set_style_bg_opa(cont, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(cont, 0, 0);

    lv_obj_set_size(cont, lv_pct(100), lv_pct(100));

    // Flex layout
    lv_obj_set_flex_flow(cont, flow);
    lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);

    // Padding + scroll
    lv_obj_set_style_pad_all(cont, 5, 0);
    lv_obj_set_scroll_dir(cont, LV_DIR_VER);
    lv_obj_set_scrollbar_mode(cont, LV_SCROLLBAR_MODE_AUTO);

    return cont;
}

lv_obj_t *RumpusGigaDisplayScreen::createCenterLayout(lv_obj_t *parent, lv_flex_flow_t flow)
{
    if (!parent)
        parent = lv_scr_act();

    lv_obj_t *cont = lv_obj_create(parent);

    // Full width, content height
    lv_obj_set_width(cont, lv_pct(100));
    lv_obj_set_height(cont, LV_SIZE_CONTENT);

    // Transparent background
    lv_obj_set_style_bg_opa(cont, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(cont, 0, 0);

    // Flex layout
    lv_obj_set_flex_flow(cont, flow);
    lv_obj_set_flex_align(cont,
                          LV_FLEX_ALIGN_CENTER, // main axis
                          LV_FLEX_ALIGN_CENTER, // cross axis
                          LV_FLEX_ALIGN_CENTER);

    // Padding + scroll if content exceeds
    lv_obj_set_style_pad_all(cont, 5, 0);
    lv_obj_set_scroll_dir(cont, LV_DIR_VER);
    lv_obj_set_scrollbar_mode(cont, LV_SCROLLBAR_MODE_AUTO);

    // Center container in parent
    lv_obj_center(cont);

    return cont;
}

// ------------------- New Widgets -------------------

// Slider
lv_obj_t *RumpusGigaDisplayScreen::createSlider(lv_obj_t *parent, int16_t min, int16_t max, int16_t value, lv_event_cb_t event_cb)
{
    if (!parent)
        parent = lv_scr_act();
    lv_obj_t *slider = lv_slider_create(parent);
    lv_slider_set_range(slider, min, max);
    lv_slider_set_value(slider, value, LV_ANIM_OFF);
    lv_obj_set_width(slider, 200);
    if (event_cb)
        lv_obj_add_event_cb(slider, event_cb, LV_EVENT_VALUE_CHANGED, nullptr);
    return slider;
}

// Switch
lv_obj_t *RumpusGigaDisplayScreen::createSwitch(lv_obj_t *parent, bool on, lv_event_cb_t event_cb)
{
    if (!parent)
        parent = lv_scr_act();

    lv_obj_t *sw = lv_switch_create(parent);

    if (on)
    {
        lv_obj_add_state(sw, LV_STATE_CHECKED);
    }
    else
    {
        lv_obj_clear_state(sw, LV_STATE_CHECKED);
    }

    if (event_cb)
        lv_obj_add_event_cb(sw, event_cb, LV_EVENT_VALUE_CHANGED, nullptr);

    return sw;
}

// Progress bar
lv_obj_t *RumpusGigaDisplayScreen::createProgressBar(lv_obj_t *parent, int16_t min, int16_t max, int16_t value)
{
    if (!parent)
        parent = lv_scr_act();
    lv_obj_t *bar = lv_bar_create(parent);
    lv_bar_set_range(bar, min, max);
    lv_bar_set_value(bar, value, LV_ANIM_OFF);
    lv_obj_set_size(bar, 200, 20);
    return bar;
}

// Text area
lv_obj_t *RumpusGigaDisplayScreen::createTextArea(lv_obj_t *parent, const char *text, lv_event_cb_t event_cb)
{
    if (!parent)
        parent = lv_scr_act();
    lv_obj_t *ta = lv_textarea_create(parent);
    lv_textarea_set_text(ta, text);
    lv_obj_set_width(ta, 200);
    if (event_cb)
        lv_obj_add_event_cb(ta, event_cb, LV_EVENT_VALUE_CHANGED, nullptr);
    return ta;
}
