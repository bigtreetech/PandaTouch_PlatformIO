#pragma once
#include <lvgl.h>
#include "pt/pt_display.h"

// Callback for brightness slider
static void pt_demo_brightness_slider_event_cb(lv_event_t *e)
{
    lv_obj_t *slider = (lv_obj_t *)lv_event_get_target(e);
    int16_t percent = lv_slider_get_value(slider);
    pt_set_backlight((uint8_t)percent, true);
}

// Create a black screen and a brightness slider
inline void pt_demo_create_brightness_demo()
{
    lv_obj_t *scr = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr, lv_color_black(), LV_PART_MAIN);
    lv_scr_load(scr);

    // Create slider for brightness (0-100)
    lv_obj_t *slider = lv_slider_create(scr);
    lv_obj_set_width(slider, 300);
    lv_obj_align(slider, LV_ALIGN_CENTER, 0, 0);
    lv_slider_set_range(slider, 20, 100);
    lv_slider_set_value(slider, pt_backlight_percent, LV_ANIM_OFF);
    lv_obj_add_event_cb(slider, pt_demo_brightness_slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    // Label for slider value
    lv_obj_t *label = lv_label_create(scr);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, -40);
    lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
    lv_label_set_text_fmt(label, "Brightness: %d%%", pt_backlight_percent);

    // Update label when slider moves
    lv_obj_add_event_cb(slider, [](lv_event_t *e)
                        {
        lv_obj_t *slider = (lv_obj_t *)lv_event_get_target(e);
        lv_obj_t *label = lv_obj_get_child(lv_obj_get_parent(slider), 1); // assumes label is second child
        int16_t percent = lv_slider_get_value(slider);
        lv_label_set_text_fmt(label, "Brightness: %d%%", percent); }, LV_EVENT_VALUE_CHANGED, NULL);
}
