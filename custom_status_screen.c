#include <zephyr/kernel.h>
#include <lvgl.h>
#include <zmk/battery.h>

lv_obj_t *zmk_display_status_screen() {
    lv_obj_t *screen = lv_obj_create(NULL);
    
    lv_obj_t *hello_label = lv_label_create(screen);
    lv_label_set_text(hello_label, "Hello MKBD!");
    lv_obj_align(hello_label, LV_ALIGN_CENTER, 0, 0);
    
    lv_obj_t *battery_label = lv_label_create(screen);
    uint8_t battery_level = zmk_battery_state_of_charge();
    lv_label_set_text_fmt(battery_label, "%d%%", battery_level);
    lv_obj_align(battery_label, LV_ALIGN_BOTTOM_LEFT, 5, -5);
    
    return screen;
}