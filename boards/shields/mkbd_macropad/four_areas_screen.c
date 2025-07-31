#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <lvgl.h>
#include <zmk/display.h>
#include "widgets/s0_widget.h"
#include "widgets/s1_widget.h"
#include "widgets/s2_widget.h"
#include "widgets/s3_widget.h"

LOG_MODULE_REGISTER(four_areas_screen, LOG_LEVEL_DBG);

static struct zmk_widget_s0 s0_widget;
static struct zmk_widget_s1 s1_widget;
static struct zmk_widget_s2 s2_widget;
static struct zmk_widget_s3 s3_widget;

lv_obj_t *zmk_display_status_screen() {
    lv_obj_t *screen = lv_obj_create(NULL);
    
    // Initialize S0 widget (Top left)
    zmk_widget_s0_init(&s0_widget, screen);
    lv_obj_t *s0_obj = zmk_widget_s0_obj(&s0_widget);
    lv_obj_align(s0_obj, LV_ALIGN_TOP_LEFT, 2, 2);
    
    // Initialize S1 widget (Top right)
    zmk_widget_s1_init(&s1_widget, screen);
    lv_obj_t *s1_obj = zmk_widget_s1_obj(&s1_widget);
    lv_obj_align(s1_obj, LV_ALIGN_TOP_RIGHT, -2, 2);
    
    // Initialize S2 widget (Bottom left)
    zmk_widget_s2_init(&s2_widget, screen);
    lv_obj_t *s2_obj = zmk_widget_s2_obj(&s2_widget);
    lv_obj_align(s2_obj, LV_ALIGN_BOTTOM_LEFT, 2, -2);
    
    // Initialize S3 widget (Bottom right)
    zmk_widget_s3_init(&s3_widget, screen);
    lv_obj_t *s3_obj = zmk_widget_s3_obj(&s3_widget);
    lv_obj_align(s3_obj, LV_ALIGN_BOTTOM_RIGHT, -2, -2);
    
    return screen;
}