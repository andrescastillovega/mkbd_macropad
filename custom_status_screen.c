#include <zephyr/kernel.h>
#include <lvgl.h>

lv_obj_t *zmk_display_status_screen() {
    lv_obj_t *screen = lv_obj_create(NULL);
    
    lv_obj_t *hello_label = lv_label_create(screen);
    lv_label_set_text(hello_label, "Hello MKBD!");
    lv_obj_align(hello_label, LV_ALIGN_CENTER, 0, 0);
    
    return screen;
}