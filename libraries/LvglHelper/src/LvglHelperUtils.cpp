#include "LvglHelperUtils.h"

LvglHelperUtils::Spinner LvglHelperUtils::createLvglSpinner(
    lv_obj_t *parent,
    uint16_t size,
    lv_color_t color)
{
    Spinner spinner;

    // Create the arc
    spinner.arc = lv_arc_create(parent);
    lv_arc_set_range(spinner.arc, 0, 360);
    lv_arc_set_value(spinner.arc, 90); // starting angle
    lv_obj_set_size(spinner.arc, size, size);

    // Style the arc
    lv_obj_set_style_arc_width(spinner.arc, 4, LV_PART_MAIN);
    lv_obj_set_style_arc_color(spinner.arc, color, LV_PART_INDICATOR);
    lv_obj_set_style_bg_opa(spinner.arc, LV_OPA_TRANSP, LV_PART_MAIN);

    lv_obj_center(spinner.arc);

    // Initialize animation
    lv_anim_init(&spinner.anim);
    lv_anim_set_var(&spinner.anim, spinner.arc);
    lv_anim_set_exec_cb(
        &spinner.anim,
        [](void *obj, int32_t v)
        {
            lv_arc_set_start_angle((lv_obj_t *)obj, v);
        });
    lv_anim_set_values(&spinner.anim, 0, 360);
    lv_anim_set_time(&spinner.anim, 1000);
    lv_anim_set_repeat_count(&spinner.anim, LV_ANIM_REPEAT_INFINITE);
    lv_anim_start(&spinner.anim);

    return spinner;
}

void LvglHelperUtils::deleteLvglSpinner(Spinner &spinner)
{
    lv_anim_del(
        spinner.arc,
        [](void *obj, int32_t v)
        {
            lv_arc_set_start_angle((lv_obj_t *)obj, v);
        });
    lv_obj_del(spinner.arc);
    spinner.arc = nullptr;
}