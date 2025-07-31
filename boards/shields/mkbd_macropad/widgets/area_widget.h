#pragma once
#include <lvgl.h>
#include <zephyr/kernel.h>

struct zmk_widget_area {
    sys_snode_t node;
    lv_obj_t *obj;
    int area_number;
};

int zmk_widget_area_init(struct zmk_widget_area *widget, lv_obj_t *parent, int area_number);
lv_obj_t *zmk_widget_area_obj(struct zmk_widget_area *widget);