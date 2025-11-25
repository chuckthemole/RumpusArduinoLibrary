#ifndef DIAGNOSTIC_H
#define DIAGNOSTIC_H

#include <Arduino.h>
#include <lvgl.h>
#include <Utils.h>

/*
 * Diagnostic overlay for LVGL
 *
 * Notes:
 * 1. LVGL heap monitoring (LV_USE_MEM_MONITOR) must be enabled in lv_conf.h:
 *       #define LV_USE_MEM_MONITOR 1
 *    and a heap size allocated:
 *       #define LV_MEM_SIZE (32*1024)  // adjust as needed
 *    Otherwise, LVGL memory usage will show 0B/0B.
 *
 * 2. The 'Refresh' field indicates whether the LVGL display refresh timer
 *    is active. It does not measure FPS directly, but if refresh is off,
 *    LVGL won’t automatically redraw and FPS estimates may be inaccurate.
 *
 * 3. Call lv_init() and register the display driver before using Diagnostic::updateDefault().
 */
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

        // Initial text
        lv_label_set_text(diag_label, "Tick:... | FPS:...");

        // Font and color
        lv_obj_set_style_text_font(diag_label, lv_theme_get_font_small(diag_label), 0);
        lv_obj_set_style_text_color(diag_label, lv_color_hex(0x00FF00), 0);

        // Optional translucent background
        lv_obj_set_style_bg_color(diag_label, lv_color_hex(0x000000), 0);
        lv_obj_set_style_bg_opa(diag_label, LV_OPA_50, 0);

        // Right-justified within the label
        lv_label_set_long_mode(diag_label, LV_LABEL_LONG_CLIP);
        lv_obj_set_style_text_align(diag_label, LV_TEXT_ALIGN_RIGHT, 0);

        // Align top-right, offset down from screen top (so above built-in overlays)
        lv_obj_align(diag_label, LV_ALIGN_BOTTOM_RIGHT, -10, -20);
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

    inline const char *formatBytes(uint32_t bytes, char *buf, size_t bufSize)
    {
        if (bytes < 1024)
        {
            snprintf(buf, bufSize, "%luB", static_cast<unsigned long>(bytes));
        }
        else if (bytes < 1024 * 1024)
        {
            float kb = bytes / 1024.0f;
            snprintf(buf, bufSize, "%.1fKB", kb);
        }
        else
        {
            float mb = bytes / (1024.0f * 1024.0f);
            snprintf(buf, bufSize, "%.2fMB", mb);
        }
        return buf;
    }

    inline void updateDefault()
    {
        if (!ready)
            createOverlay();
        if (!ready)
            return;

        lv_display_t *disp = lv_display_get_default();
        if (!disp)
            return;

        uint32_t tick = lv_tick_get();

        // FPS estimate (moving average)
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

        // LVGL memory
        uint32_t lv_mem_used = 0, lv_mem_free = 0;
#if LV_USE_MEM_MONITOR
        lv_mem_monitor_t mon;
        lv_mem_monitor(&mon);
        lv_mem_free = mon.free_size;
        lv_mem_used = (mon.total_size - mon.free_size);
#endif

        int free_ram = freeRAM(); // heap outside LVGL

        // Format human-readable fields
        char ramBuf[16], lvUsedBuf[16], lvFreeBuf[16];
        formatBytes(free_ram, ramBuf, sizeof(ramBuf));
        formatBytes(lv_mem_used, lvUsedBuf, sizeof(lvUsedBuf));
        formatBytes(lv_mem_free, lvFreeBuf, sizeof(lvFreeBuf));

        lv_coord_t hor = lv_display_get_horizontal_resolution(disp);
        lv_coord_t ver = lv_display_get_vertical_resolution(disp);
        uint32_t dpi = lv_display_get_dpi(disp);

        char buf[256];
        snprintf(buf, sizeof(buf),
                 "Tick:%lu | FPS:%.1f | LVGL:%s/%s | RAM:%s | %dx%d %luDPI | Refresh:%s",
                 static_cast<unsigned long>(tick),
                 fps_estimate,
                 lvUsedBuf,
                 lvFreeBuf,
                 ramBuf,
                 static_cast<int>(hor),
                 static_cast<int>(ver),
                 static_cast<unsigned long>(dpi),
                 has_refresh ? "yes" : "no");

        lv_label_set_text(diag_label, buf);
        lv_obj_invalidate(diag_label);
    }

} // namespace Diagnostic

#endif // DIAGNOSTIC_H
