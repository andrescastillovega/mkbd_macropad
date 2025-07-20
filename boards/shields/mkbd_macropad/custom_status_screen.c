#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <lvgl.h>
#include <zmk/battery.h>
#include <zmk/usb.h>
#include <zmk/display.h>
#include <zmk/event_manager.h>
#include <zmk/events/usb_conn_state_changed.h>
#include <zmk/events/battery_state_changed.h>
#include "usb_img.h"
#include "battery_img.h"
#if IS_ENABLED(CONFIG_ZMK_WIDGET_BATTERY_ANIM)
#include "widgets/battery_anim.h"
#endif

LOG_MODULE_REGISTER(custom_status_screen, LOG_LEVEL_DBG);

static lv_obj_t *status_image;
#if IS_ENABLED(CONFIG_ZMK_WIDGET_BATTERY_ANIM)
static struct zmk_widget_battery_anim battery_anim_widget;
#endif

struct status_state {
    uint8_t battery_level;
    bool usb_connected;
};

static struct status_state get_status_state(const zmk_event_t *eh) {
    return (struct status_state){
        .battery_level = zmk_battery_state_of_charge(),
        .usb_connected = zmk_usb_is_powered(),
    };
}

static void update_status_display(struct status_state state) {
    if (status_image == NULL) return;
    
    if (state.usb_connected) {
        lv_img_set_src(status_image, &usb_img);
        printk("USB connected - Battery: %d%%\n", state.battery_level);
    } else {
        lv_img_set_src(status_image, &battery_img);
        printk("Battery: %d%%\n", state.battery_level);
    }
}

ZMK_DISPLAY_WIDGET_LISTENER(status_screen, struct status_state, 
                            update_status_display, get_status_state)

ZMK_SUBSCRIPTION(status_screen, zmk_usb_conn_state_changed);
ZMK_SUBSCRIPTION(status_screen, zmk_battery_state_changed);

lv_obj_t *zmk_display_status_screen() {
    lv_obj_t *screen = lv_obj_create(NULL);
    
    status_image = lv_img_create(screen);
    lv_obj_align(status_image, LV_ALIGN_CENTER, 0, 0);
    
#if IS_ENABLED(CONFIG_ZMK_WIDGET_BATTERY_ANIM)
    // Initialize battery animation widget in top right corner with 12x4 pixel offset
    zmk_widget_battery_anim_init(&battery_anim_widget, screen);
    lv_obj_t *battery_anim_obj = zmk_widget_battery_anim_obj(&battery_anim_widget);
    lv_obj_align(battery_anim_obj, LV_ALIGN_TOP_RIGHT, -12, 4);
#endif
    
    status_screen_init();
    
    struct status_state initial_state = get_status_state(NULL);
    update_status_display(initial_state);
    
    return screen;
}