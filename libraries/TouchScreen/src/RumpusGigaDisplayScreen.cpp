#include "RumpusGigaDisplayScreen.h"

// ------------------ Singleton instance ------------------
RumpusGigaDisplayScreen *RumpusGigaDisplayScreen::_instance = nullptr;

// ------------------ Constructor / Destructor ------------------
RumpusGigaDisplayScreen::RumpusGigaDisplayScreen(RumpshiftLogger *logger,
                                                 uint16_t horRes,
                                                 uint16_t verRes,
                                                 uint16_t bufHeight)
    : _logger(logger),
      _horRes(horRes),
      _verRes(verRes),
      _bufHeight(bufHeight == 0 ? 100 : bufHeight),
      _buf1(nullptr),
      _buf2(nullptr),
      _darkMode(false),
      _disp(nullptr),
      _lastButtonPress(0)
{
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
    if (_logger)
        _logger->info("Display init...");
    lv_init();

    _display.begin();
    _touch.begin();

    // Allocate LVGL buffers
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
    _disp = lv_disp_drv_register(&disp_drv);

    // Touch/input driver
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touch_read_cb;
    lv_indev_drv_register(&indev_drv);

    if (_logger)
        _logger->info("LVGL init done.");
    return true;
}

// ------------------ Main LVGL loop ------------------
void RumpusGigaDisplayScreen::loop()
{
    lv_timer_handler();
    delay(5);
}

// ------------------ LVGL Callbacks ------------------
void RumpusGigaDisplayScreen::flush_cb(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
    if (!_instance)
        return;

    _instance->_display.drawRGBBitmap(
        area->x1, area->y1,
        reinterpret_cast<uint16_t *>(color_p),
        area->x2 - area->x1 + 1,
        area->y2 - area->y1 + 1);

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

// ------------------ Widget creation helpers ------------------
lv_obj_t *RumpusGigaDisplayScreen::createLabel(const char *text, lv_obj_t *parent, const lv_font_t *font)
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

lv_obj_t *RumpusGigaDisplayScreen::createCenteredLabel(const char *text, lv_obj_t *parent, const lv_font_t *font)
{
    lv_obj_t *label = createLabel(text, parent, font);
    lv_obj_center(label);
    return label;
}

lv_obj_t *RumpusGigaDisplayScreen::createButton(const char *text, lv_obj_t *parent, lv_event_cb_t event_cb, const lv_font_t *font)
{
    if (!parent)
        parent = lv_scr_act();
    lv_obj_t *btn = lv_btn_create(parent);
    if (event_cb)
        lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, nullptr);

    lv_obj_t *label = createLabel(text, btn, font);
    lv_obj_center(label);
    return btn;
}

// ------------------ Debounced Button ------------------
lv_obj_t *RumpusGigaDisplayScreen::createDebouncedButton(const char *text, lv_obj_t *parent, std::function<void()> cb, const lv_font_t *font)
{
    struct ButtonCbData
    {
        std::function<void()> cb;
    };

    static auto debounced_cb = [](lv_event_t *e)
    {
        if (!_instance)
            return;
        uint32_t now = millis();
        if (now - _instance->_lastButtonPress < _instance->_buttonCooldown)
            return;
        _instance->_lastButtonPress = now;

        ButtonCbData *data = static_cast<ButtonCbData *>(lv_event_get_user_data(e));
        if (data && data->cb)
            data->cb();
    };

    if (!parent)
        parent = lv_scr_act();
    ButtonCbData *data = new ButtonCbData{cb};
    lv_obj_t *btn = lv_btn_create(parent);
    lv_obj_add_event_cb(btn, debounced_cb, LV_EVENT_CLICKED, data);

    lv_obj_t *label = createLabel(text, btn, font);
    lv_obj_center(label);

    return btn;
}

// ------------------ Containers ------------------
lv_obj_t *RumpusGigaDisplayScreen::createGridContainer(lv_obj_t *parent, const lv_coord_t *row_dsc, const lv_coord_t *col_dsc, uint8_t row_count, uint8_t col_count, lv_align_t align)
{
    if (!parent)
        parent = lv_scr_act();
    lv_obj_t *grid = lv_obj_create(parent);
    lv_obj_set_size(grid, lv_pct(100), lv_pct(100));
    lv_obj_set_layout(grid, LV_LAYOUT_GRID);
    lv_obj_set_grid_dsc_array(grid, col_dsc, row_dsc);
    lv_obj_align(grid, align, 0, 0);
    return grid;
}

lv_obj_t *RumpusGigaDisplayScreen::createFlexContainer(lv_obj_t *parent, lv_flex_flow_t flow)
{
    if (!parent)
        parent = lv_scr_act();
    lv_obj_t *cont = lv_obj_create(parent);
    lv_obj_set_size(cont, lv_pct(100), lv_pct(100));
    lv_obj_set_flex_flow(cont, flow);
    return cont;
}

lv_obj_t *RumpusGigaDisplayScreen::createCenterLayout(lv_obj_t *parent, lv_flex_flow_t flow)
{
    lv_obj_t *cont = createFlexContainer(parent, flow);
    lv_obj_center(cont);
    return cont;
}

// ------------------ Sliders / Switches / Progress ------------------
lv_obj_t *RumpusGigaDisplayScreen::createSlider(lv_obj_t *parent, int16_t min, int16_t max, int16_t value, lv_event_cb_t event_cb)
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

lv_obj_t *RumpusGigaDisplayScreen::createSwitch(lv_obj_t *parent, bool on, lv_event_cb_t event_cb)
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

lv_obj_t *RumpusGigaDisplayScreen::createProgressBar(lv_obj_t *parent, int16_t min, int16_t max, int16_t value)
{
    if (!parent)
        parent = lv_scr_act();
    lv_obj_t *bar = lv_bar_create(parent);
    lv_bar_set_range(bar, min, max);
    lv_bar_set_value(bar, value, LV_ANIM_OFF);
    return bar;
}

lv_obj_t *RumpusGigaDisplayScreen::createTextArea(lv_obj_t *parent, const char *text, lv_event_cb_t event_cb, const lv_font_t *font)
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

void RumpusGigaDisplayScreen::applySimpleTheme()
{
    static lv_theme_t *theme = lv_theme_default_init(
        nullptr, lv_palette_main(LV_PALETTE_BLUE), true, LV_FONT_DEFAULT);
    lv_disp_set_theme(_disp, theme);
}

void RumpusGigaDisplayScreen::showMainMenu(std::function<void(const char *)> modeSelectedCb)
{
    clear();
    lv_obj_t *cont = createCenterLayout();
    createDebouncedButton("Fill Mode", cont, [modeSelectedCb]()
                          { modeSelectedCb("Fill Mode"); });
    createDebouncedButton("Temp Mode Only", cont, [modeSelectedCb]()
                          { modeSelectedCb("Temp Mode Only"); });
}

void RumpusGigaDisplayScreen::showFillMenu(float threshold,
                                           std::function<void(float)> thresholdChangedCb,
                                           std::function<void()> startFlowCb)
{
    clear();
    lv_obj_t *cont = createCenterLayout();

    // Slider for threshold
    lv_obj_t *slider = createSlider(cont, 0, 10, (int)threshold, [](lv_event_t *e)
                                    {
                                        lv_obj_t *s = lv_event_get_target(e);
                                        int val = lv_slider_get_value(s);
                                        thresholdChangedCb((float)val); });
    // Start Flow button
    createDebouncedButton("Start Flow", cont, startFlowCb);
    // Home button
    createDebouncedButton("Home", cont, goHome);
}

void RumpusGigaDisplayScreen::showTempScreen()
{
    clear();
    createCenteredLabel("Temperature Screen");
    createDebouncedButton("Home", nullptr, goHome);
}
