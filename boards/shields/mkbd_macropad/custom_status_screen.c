#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <lvgl.h>
#include <zmk/battery.h>
#include <zmk/usb.h>
#include <zmk/display.h>
#include <zmk/event_manager.h>
#include <zmk/events/usb_conn_state_changed.h>
#include <zmk/events/battery_state_changed.h>
#include <zmk/events/split_peripheral_status_changed.h>
#include <zmk/split/central.h>

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

static lv_obj_t *main_label;
static lv_timer_t *fade_timer;
// static lv_timer_t *hello_timer;

struct status_state {
    uint8_t battery_level_left;
    bool usb_connected_left;
    uint8_t battery_level_right;
};

static struct status_state get_status_state(const zmk_event_t *eh) {
    uint8_t battery_level_left = zmk_battery_state_of_charge();
    bool usb_connected_left = zmk_usb_is_powered();
    uint8_t battery_level_right = 0;
    int ret = zmk_split_central_get_peripheral_battery_level(0, &battery_level_right);
    
    // Log battery status update
    LOG_INF("Battery status - Left side: %d%%, USB: %s", 
            battery_level_left, usb_connected_left ? "connected" : "disconnected");
    LOG_INF("Battery status - Right side: %d%%", battery_level_right);
    
    // Get right side (peripheral) battery level
    if (ret == 0) {
        LOG_INF("Right side battery: %d%%", battery_level_right);
    } else {
        LOG_INF("Right side battery: unavailable (error %d)", ret);
    }
    
    return (struct status_state){
        .battery_level_left = battery_level_left,
        .usb_connected_left = usb_connected_left,
        .battery_level_right = battery_level_right,
    };
}

static void update_status_display(struct status_state state) {
    if (main_label == NULL) return;
    
    char status_text[32];
    if (state.usb_connected_left) {
        snprintf(status_text, sizeof(status_text), "Charging...");
    } else {
        snprintf(status_text, sizeof(status_text), "L: %d%% R:%d%%", state.battery_level_left, state.battery_level_right);
    }
    
    lv_label_set_text(main_label, status_text);
}

static void fade_to_battery_status(lv_timer_t *timer) {
    struct status_state state = get_status_state(NULL);
    update_status_display(state);
    lv_timer_del(fade_timer);
    fade_timer = NULL;
}

// static void print_hello_world(lv_timer_t *timer) {
//     LOG_INF("Hello world!");
// }

static int split_peripheral_status_listener(const zmk_event_t *eh) {
    struct zmk_split_peripheral_status_changed *ev = as_zmk_split_peripheral_status_changed(eh);
    if (ev == NULL) {
        return ZMK_EV_EVENT_BUBBLE;
    }
    
    if (ev->connected) {
        LOG_INF("Right side connected to left side!");
    } else {
        LOG_INF("Right side disconnected from left side.");
    }

    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_DISPLAY_WIDGET_LISTENER(status_screen, struct status_state, 
                            update_status_display, get_status_state)

ZMK_SUBSCRIPTION(status_screen, zmk_usb_conn_state_changed);
ZMK_SUBSCRIPTION(status_screen, zmk_battery_state_changed);

ZMK_LISTENER(split_status_listener, split_peripheral_status_listener);
ZMK_SUBSCRIPTION(split_status_listener, zmk_split_peripheral_status_changed);

lv_obj_t *zmk_display_status_screen() {
    lv_obj_t *screen = lv_obj_create(NULL);
    
    main_label = lv_label_create(screen);
    lv_label_set_text(main_label, "Hello P1!");
    lv_obj_align(main_label, LV_ALIGN_CENTER, 0, 0);
    
    status_screen_init();
    
    fade_timer = lv_timer_create(fade_to_battery_status, 5000, NULL);
    lv_timer_set_repeat_count(fade_timer, 1);
    
    // hello_timer = lv_timer_create(print_hello_world, 1000, NULL);
    
    return screen;
}