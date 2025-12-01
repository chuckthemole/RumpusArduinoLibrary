#ifndef LVGL_HELPER_UTILS_H
#define LVGL_HELPER_UTILS_H

#include <lvgl.h>

namespace LvglHelperUtils
{
    struct Spinner
    {
        lv_obj_t *arc;
        lv_anim_t anim;
    };

    /**
     * @brief Creates a spinning arc (spinner) on the given parent object.
     *
     * @param parent The LVGL parent object.
     * @param size Diameter of the spinner in pixels.
     * @param color Color of the spinner arc.
     * @return Spinner struct containing the arc and animation handle.
     */
    Spinner createLvglSpinner(lv_obj_t *parent, uint16_t size = 50, lv_color_t color = lv_color_hex(0x00AAFF));

    /**
     * @brief Stops and deletes the spinner
     */
    void deleteLvglSpinner(Spinner &spinner);

} // namespace LvglHelperUtils

#endif // LVGL_HELPER_UTILS_H
