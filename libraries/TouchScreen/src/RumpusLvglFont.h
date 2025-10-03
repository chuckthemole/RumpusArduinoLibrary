#pragma once
#include <lvgl.h>

// --- Declare the fonts (these are compiled in LVGL) ---
LV_FONT_DECLARE(lv_font_montserrat_14);
LV_FONT_DECLARE(lv_font_montserrat_24);
LV_FONT_DECLARE(lv_font_montserrat_32);

// --- Optional aliases for easy usage ---
static constexpr const lv_font_t *DEFAULT_FONT_SMALL = &lv_font_montserrat_14;
static constexpr const lv_font_t *DEFAULT_FONT_MEDIUM = &lv_font_montserrat_24;
static constexpr const lv_font_t *DEFAULT_FONT_LARGE = &lv_font_montserrat_32;
