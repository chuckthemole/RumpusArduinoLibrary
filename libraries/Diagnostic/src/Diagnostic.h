#ifndef DIAGNOSTIC_H
#define DIAGNOSTIC_H

#include <Arduino.h>
#include <lvgl.h>

namespace Diagnostic
{
    static lv_obj_t *diag_label = nullptr;
    static uint32_t last_tick = 0;
    static float fps_estimate = 0.0f;
    static bool ready = false;

    /**
     * @brief Check if LVGL is initialized and ready.
     */
    inline bool isReady()
    {
        lv_display_t *disp = lv_display_get_default();
        return (disp != nullptr);
    }

    /**
     * @brief Create the diagnostic overlay label on the top layer (always visible across screens).
     */
    inline void createOverlay()
    {
        if (diag_label || !isReady())
            return;

        ready = true;

        lv_obj_t *parent = lv_layer_top(); // top layer, survives screen changes
        diag_label = lv_label_create(parent);
        lv_label_set_text(diag_label, "Diag: init...");
        lv_obj_set_style_text_font(diag_label, lv_theme_get_font_small(diag_label), 0);
        lv_obj_set_style_text_color(diag_label, lv_color_hex(0x00FF00), 0);

        // Optional translucent background for readability
        lv_obj_set_style_bg_color(diag_label, lv_color_hex(0x000000), 0);
        lv_obj_set_style_bg_opa(diag_label, LV_OPA_50, 0);

        lv_obj_align(diag_label, LV_ALIGN_BOTTOM_RIGHT, -10, -10);
    }

    /**
     * @brief Update overlay text with a custom message.
     */
    inline void update(const char *msg)
    {
        if (!ready)
        {
            createOverlay();
            if (!ready)
                return;
        }

        lv_label_set_text(diag_label, msg);
        lv_obj_invalidate(diag_label);
    }

    /**
     * @brief Update overlay with tick, FPS estimate, memory, and display stats.
     */
    inline void updateDefault()
    {
        if (!ready)
        {
            createOverlay();
            if (!ready)
                return;
        }

        lv_display_t *disp = lv_display_get_default();
        if (!disp)
            return;

        uint32_t tick = lv_tick_get();

        // Calculate FPS (simple moving average)
        if (last_tick != 0)
        {
            uint32_t delta = tick - last_tick;
            if (delta > 0 && delta < 200)
            {
                float current_fps = 1000.0f / delta;
                fps_estimate = fps_estimate * 0.9f + current_fps * 0.1f;
            }
        }
        last_tick = tick;

        bool has_refresh = lv_display_get_refr_timer(disp) != nullptr;

        uint32_t mem_used_kb = 0, mem_free_kb = 0;
#if LV_USE_MEM_MONITOR
        lv_mem_monitor_t mon;
        lv_mem_monitor(&mon);
        mem_free_kb = mon.free_size / 1024;
        mem_used_kb = (mon.total_size - mon.free_size) / 1024;
#endif

        lv_coord_t hor = lv_display_get_horizontal_resolution(disp);
        lv_coord_t ver = lv_display_get_vertical_resolution(disp);
        uint32_t dpi = lv_display_get_dpi(disp);

        char buf[192];
        snprintf(buf, sizeof(buf),
                 "Tick:%lu | FPS:%.1f | Refresh:%s | Mem:%lu/%luKB | %dx%d %luDPI",
                 static_cast<unsigned long>(tick),
                 fps_estimate,
                 has_refresh ? "yes" : "no",
                 static_cast<unsigned long>(mem_used_kb),
                 static_cast<unsigned long>(mem_free_kb),
                 static_cast<int>(hor),
                 static_cast<int>(ver),
                 static_cast<unsigned long>(dpi));

        lv_label_set_text(diag_label, buf);
        lv_obj_invalidate(diag_label);
    }

} // namespace Diagnostic

#endif // DIAGNOSTIC_H
