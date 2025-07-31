#pragma once
#include <lvgl.h>
#include <zephyr/kernel.h>

struct zmk_widget_s1 {
    sys_snode_t node;
    lv_obj_t *obj;
};

int zmk_widget_s1_init(struct zmk_widget_s1 *widget, lv_obj_t *parent);
lv_obj_t *zmk_widget_s1_obj(struct zmk_widget_s1 *widget);