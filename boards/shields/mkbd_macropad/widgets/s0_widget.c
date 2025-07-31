#include <zephyr/kernel.h>
#include <zmk/display.h>
#include <zmk/event_manager.h>
#include "s0_widget.h"

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);

struct widget_state {
    int dummy;
};

static void widget_update_cb(struct widget_state state) {
    // No dynamic updates needed for static text
}

static struct widget_state get_state(const zmk_event_t *eh) {
    return (struct widget_state){
        .dummy = 0,
    };
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_s0, struct widget_state,
                            widget_update_cb, get_state)

int zmk_widget_s0_init(struct zmk_widget_s0 *widget, lv_obj_t *parent) {
    widget->obj = lv_label_create(parent);
    
    lv_label_set_text(widget->obj, "S0-Hello World!");
    lv_obj_set_style_text_align(widget->obj, LV_TEXT_ALIGN_CENTER, 0);
    
    sys_slist_append(&widgets, &widget->node);
    widget_s0_init();
    
    return 0;
}

lv_obj_t *zmk_widget_s0_obj(struct zmk_widget_s0 *widget) {
    return widget->obj;
}